/***************************************************************************
                          standardtext.cpp  -  description
                             -------------------
    copyright            : (C) 2003 by Scott Wheeler
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

#include "standardtext.h"

#include <ktextedit.h>

#include <qlabel.h>

StandardText::StandardText(QWidget *parent) : QDialog(parent)
{
   setupUi(this);
   setModal(true);
   connect(buttonOk,SIGNAL(clicked()),this,SLOT(slotAccept()));
   connect(buttonCancel,SIGNAL(clicked()),this,SLOT(reject()));
}

StandardText::~StandardText()
{

}

void StandardText::setLabel(const QString &text)
{
    label->setText(text);
}

void StandardText::setText(const QString &text)
{
    textEdit->setText(text);
}

void StandardText::slotAccept()
{
   emit textUpdated(textEdit->toPlainText());
   accept();
}

#include "standardtext.moc"

