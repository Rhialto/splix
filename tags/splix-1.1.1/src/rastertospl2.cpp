/*
 * 	rastertospl2.cpp	(C) 2006, Aurélien Croc (AP²C)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 * 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the
 *  Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *  $Id$
 * 
 */

#include "version.h"
#include "printer.h"
#include "raster.h"
#include "spl2.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <cups/ppd.h>
#include <cups/cups.h>
#include <sys/resource.h>

int main(int argc, char **argv)
{
    cups_option_t *options;
    Raster *document;
    Printer *printer;
    ppd_file_t* ppd;
    SPL2 spl2;
    int nr;

    setbuf(stderr, NULL);
    setbuf(stdout, NULL);
#ifdef ENABLE_DEBUG
    freopen("/tmp/result.spl2", "w", stdout);
    struct rlimit _rlimit = {RLIM_INFINITY, RLIM_INFINITY};
    setrlimit(RLIMIT_CORE, &_rlimit);
#endif /* ENABLE_DEBUG */

    // Check if enough arguments are available
    if (argc < 6 || argc > 7) {
        fprintf(stderr, _("ERROR: %s job-id user title copies options "
            "[file]\n"), argv[0]);
        return 1;
    }

    // Create the document
    document = new Raster(argv[1], argv[2], argv[3], argv[4], 
            argv[5], argv[6]);
    if (document->load()) {
        delete document;
        return 1;
    }

    // Open the PPD file
    ppd = ppdOpenFile(getenv("PPD"));
    ppdMarkDefaults(ppd);

    // Take modifications in the PPD with options
    nr = cupsParseOptions(argv[5], 0, &options);
    cupsMarkOptions(ppd, nr, options);
    cupsFreeOptions(nr, options);

    // Check if it's a valid PPD
    ppd_attr_t *attr = ppdFindAttr(ppd, "FileVersion", NULL);
    if (!attr) {
        ERROR("No FileVersion found in the PPD file");
        return 1;
    }
    if (strcmp(attr->value, VERSION)) {
        ERROR("Invalid PPD file version: Splix V. %s but the PPD file "
            "is designed for SpliX V. %s", VERSION, attr->value);
        return 1;
    }

    // Create the printer
    printer = new Printer(ppd);
    printer->setJobName(argv[1]);
    printer->setUsername(argv[2]);

    // Convert and print
    DEBUG("Génération du code....");
    spl2.setPrinter(printer);
    spl2.setOutput(stdout);
    spl2.beginDocument();

    while (!spl2.printPage(document, strtol(argv[4], (char **)NULL, 10)));

    spl2.closeDocument();

    ppdClose(ppd);
    delete document;
    delete printer;

    return 0;
}

/* vim: set expandtab tabstop=4 shiftwidth=4 smarttab tw=80 cin enc=utf8: */

