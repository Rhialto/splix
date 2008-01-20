/*
 * 	    printer.h                 (C) 2006-2007, Aurélien Croc (AP²C)
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
#ifndef _PRINTER_H_
#define _PRINTER_H_

class Request;

/**
  * @brief This class contains all the needed information about the printer.
  *
  * This class is mainly used by the QPDL render.
  */
class Printer 
{
    protected:
        char*                   _manufacturer;
        char*                   _model;

        unsigned char           _qpdlVersion;
        unsigned long           _bandHeight;

        unsigned char           _paperType;
        unsigned char           _paperSource;



    public:
        /**
          * Initialize a new instance.
          */
        Printer();
        /**
          * Destroy the instance and free the internal memory used.
          */
        virtual ~Printer();

    public:
        /**
          * Load the printer configuration requested.
          * @return TRUE if the information have been successfully loaded.
          *         Otherwise it returns FALSE.
          */
        bool                    loadInformation(const Request& request);

    public:
        /**
          * @return the manufacturer name.
          */
        const char*             manufacturer() const {return _manufacturer;}
        /**
          * @return the model name.
          */
        const char*             model() const {return _model;}
        /**
          * @return the height of a band.
          */ 
        unsigned long           bandHeight() const {return _bandHeight;}
};

#endif /* _PRINTER_H_ */

/* vim: set expandtab tabstop=4 shiftwidth=4 smarttab tw=80 cin enc=utf8: */

