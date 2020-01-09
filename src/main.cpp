/***************************************************************************
                          main.cpp  -  description
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

#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>
#include <kdebug.h>

#include "ksig.h"
#include "siggenerator.h"
//Added by qt3to4:
#include <QTextStream>

static const char description[] = I18N_NOOP("KSig");
	
int main(int argc, char *argv[])
{

    KAboutData aboutData("ksig", 0, ki18n("KSig"),
			 "1.1", ki18n(description), KAboutData::License_GPL,
			 ki18n("(c) 2002, Scott Wheeler"), KLocalizedString(), 0, "wheeler@kde.org");

    aboutData.addAuthor(ki18n("Scott Wheeler"), KLocalizedString(), "wheeler@kde.org");
    KCmdLineArgs::init(argc, argv, &aboutData);

    KCmdLineOptions options;
    options.add("random", ki18n("Display a random signature"));
    options.add("daily", ki18n("Display a signature for the day"));
    KCmdLineArgs::addCmdLineOptions(options);

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

    if(args->isSet("random") || args->isSet("daily")) {
	KApplication a(false);
	SigGenerator generator;

	QTextStream output(stdout, QIODevice::WriteOnly);
	if(args->isSet("random"))
	    output << generator.random();
	else if(args->isSet("daily"))
	    output << generator.daily();

	return(0);
    }
    else {
	KApplication a;

	KSig *ksig = new KSig();
	a.setMainWidget(ksig);
	ksig->show();
	return a.exec();
    }
}
