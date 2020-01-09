/***************************************************************************
                          ksig.cpp  -  description
                             -------------------
    begin                : Tue Jul  9 23:14:22 EDT 2002
    copyright            : (C) 2002 by Scott Wheeler
    email                : wheeler@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "ksig.h"
#include "siglistviewitem.h"
#include "standardtext.h"

#include <kstandardaction.h>
#include <kactioncollection.h>
#include <kaction.h>
#include <kconfig.h>
#include <kglobal.h>
#include <kglobalsettings.h>
#include <kmessagebox.h>
#include <kstatusbar.h>
#include <ktextedit.h>
#include <k3listviewsearchline.h>
#include <klocale.h>

#include <qsplitter.h>
#include <kconfiggroup.h>

////////////////////////////////////////////////////////////////////////////////
// public methods
////////////////////////////////////////////////////////////////////////////////

KSig::KSig(QWidget *parent, const char *name) : KXmlGuiWindow(parent),
						changed(false)
{
    setObjectName(name);
    setupActions();
    setupLayout();
    setupSearchLine();

    if (!initialGeometrySet())
        resize(640,480);
    setupGUI(ToolBar | Keys | StatusBar | Create);
    setAutoSaveSettings();

    readConfig();
    loadData();
}

KSig::~KSig()
{

}

////////////////////////////////////////////////////////////////////////////////
// private methods
////////////////////////////////////////////////////////////////////////////////

void KSig::setupActions()
{
    KStandardAction::openNew(this, SLOT(add()), actionCollection());
    QAction *a;
    a = actionCollection()->addAction( "remove" );
    a->setText(i18n("Remove"));
    a->setIcon(KIcon("edit-delete"));
    connect(a, SIGNAL(triggered()), this, SLOT(remove()));
    KStandardAction::save(this, SLOT(save()), actionCollection());
    KStandardAction::quit(this, SLOT(quit()), actionCollection());

    KStandardAction::cut(actionCollection());
    KStandardAction::copy(actionCollection());
    KStandardAction::paste(actionCollection());
    KStandardAction::clear(actionCollection());

    a = actionCollection()->addAction( "editHeader" );

    a->setText(i18n("Edit Standard Header"));
    connect(a, SIGNAL(triggered()), this, SLOT(editHeader()));
    a = actionCollection()->addAction( "editFooter" );

    a->setText(i18n("Edit Standard Footer"));
    connect(a, SIGNAL(triggered()), this, SLOT(editFooter()));
}

void KSig::setupLayout()
{
    splitter = new QSplitter(Qt::Vertical, this);

    setCentralWidget(splitter);

    sigList = SigListView::instance(splitter, "sigList");

    sigEdit = new KTextEdit(splitter );
	sigEdit->setObjectName("sigEdit");
    //sigEdit->setTextFormat(Qt::PlainText);
    sigEdit->setFont(KGlobalSettings::fixedFont());
    sigEdit->setEnabled(false);
    sigEdit->setCheckSpellingEnabled(true);

    statusBar()->insertPermanentItem(i18n(" Line: %1 ", 0), LineNumber, 0);
    statusBar()->insertPermanentItem(i18n(" Col: %1 ", 0), ColumnNumber, 0);
    statusBar()->show();

    updateListLock = false;
    connect(sigEdit, SIGNAL(textChanged()), this, SLOT(updateList()));
    connect(sigList, SIGNAL(selectionChanged()), this, SLOT(updateEdit()));
    connect(sigEdit, SIGNAL(cursorPositionChanged(int,int)), this, SLOT(updateStatus(int,int)));
}

void KSig::setupSearchLine()
{
    K3ListViewSearchLineWidget *searchWidget = new K3ListViewSearchLineWidget(sigList, this);
    QAction *act = actionCollection()->addAction( "search" );
    act->setText( i18n("Search") );
#ifdef __GNUC__
#warning "kde4: port it"
#endif
    //act->setDefaultWidget(searchWidget);

}

void KSig::loadData()
{
    if(sigList->childCount() > 0) {
        sigList->setCurrentItem(sigList->firstChild());
        sigList->setSelected(sigList->firstChild(), true);

        sigEdit->setEnabled(true);
    }
    else {
        updateListLock = true;
        sigEdit->setText(i18n("To get started, first create a new signature by selecting "
                              "\"New\" above. You will then be able to edit and save "
                              "your collection of signatures."));
        updateListLock = false;
    }
    setDataChanged(false);
}

void KSig::readConfig()
{
    setAutoSaveSettings();
    KSharedConfig::Ptr config = KGlobal::config();
    {
	KConfigGroup group(config, "Settings");
	if(splitter) {
	    QList<int> sizes;

	    sizes.append(group.readEntry("ListSize", -1));
	    sizes.append(group.readEntry("EditSize", -1));

	    if(sizes[0] > 0 && sizes[1] > 0)
		splitter->setSizes(sizes);
	}

	header = group.readEntry("Header");
	footer = group.readEntry("Footer");
    }
}

void KSig::writeConfig()
{
    KSharedConfig::Ptr config = KGlobal::config();
    {
	KConfigGroup group(config, "Settings");
	if(splitter) {
	    QList<int> sizes = splitter->sizes();

	    group.writeEntry("ListSize", sizes[0]);
	    group.writeEntry("EditSize", sizes[1]);
	}

	group.writeEntry("Header", header);
	group.writeEntry("Footer", footer);
    }
    config->sync();
}

bool KSig::queryClose()
{
    if(changed) {
	int saveChanges = KMessageBox::questionYesNoCancel(this, i18n("Do you want to save your changes before exiting?"), QString(), KStandardGuiItem::save(), KStandardGuiItem::discard());
	if(saveChanges == KMessageBox::Cancel)
	    return(false);
	else if(saveChanges == KMessageBox::Yes)
	    save();
    }

    writeConfig();
    return(true);
}

////////////////////////////////////////////////////////////////////////////////
// private slots
////////////////////////////////////////////////////////////////////////////////

void KSig::add()
{
    setDataChanged();
    SigListViewItem *sigItem = sigList->createItem();
    sigList->setCurrentItem(sigItem);

    sigEdit->setEnabled(true);
    sigEdit->setFocus();
    sigEdit->clear();
}

void KSig::remove()
{
    setDataChanged();
    sigEdit->clear();
    delete(sigList->currentItem());
}

void KSig::save()
{
    sigList->save();
    setDataChanged(false);
}

void KSig::setDataChanged(bool value)
{
    changed = value;
    setCaption(QString::null, changed);	//krazy:exclude=nullstrassign for old broken gcc
}

void KSig::editHeader()
{
    StandardText *t = new StandardText(0);
    t->setLabel(i18n("Standard signature header:"));
    t->setText(header);

    connect(t, SIGNAL(textUpdated(const QString &)), this, SLOT(setHeader(const QString &)));

    t->exec();
}

void KSig::editFooter()
{
    StandardText *t = new StandardText(0);
    t->setLabel(i18n("Standard signature footer:"));
    t->setText(footer);

    connect(t, SIGNAL(textUpdated(const QString &)), this, SLOT(setFooter(const QString &)));

    t->exec();
}

void KSig::updateList()
{
    if(!updateListLock) {
	setDataChanged();
	SigListViewItem *item = sigList->currentItem();
	if(item)
	    item->setText(sigEdit->toPlainText());
    }
}

void KSig::updateEdit()
{
    updateListLock = true;

    SigListViewItem *item = sigList->currentItem();
    if(item)
	sigEdit->setText(item->text());

    updateListLock = false;
}

void KSig::updateStatus(int line, int column)
{
    statusBar()->changeItem(i18n(" Line: %1 ", line + 1), LineNumber);
    statusBar()->changeItem(i18n(" Col: %1 ", column + 1), ColumnNumber);
}

void KSig::quit()
{
    close();
}

#include "ksig.moc"
