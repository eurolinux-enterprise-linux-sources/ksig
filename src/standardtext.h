/***************************************************************************
                          standardtext.h  -  description
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

#ifndef STANDARDTEXT_H
#define STANDARDTEXT_H

#include "ui_standardtextbase.h"
#include <qdialog.h>

class StandardText : public QDialog,public Ui::StandardTextBase
{
    Q_OBJECT
public:
    StandardText(QWidget *parent);
    virtual ~StandardText();
    
    void setLabel(const QString &text);
    void setText(const QString &text);

signals:
    void textUpdated(const QString &);

private slots:
    void slotAccept();
};

#endif
