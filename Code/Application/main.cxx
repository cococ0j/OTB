/*=========================================================================

  Program:   Monteverdi2
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi2 is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

// Monteverdi includes (sorted by alphabetic order)
#include "mvdApplication.h"
#include "mvdMainWindow.h"
#include "mvdDatasetModel.h"

//
// OTB includes (sorted by alphabetic order)

//
// MAIN
//
int
main( int argc, char* argv[] )
{
  mvd::Application application( argc, argv );
  
  //
  // Force numeric options of locale to "C"
  // See issue #635
  //
  setlocale( LC_NUMERIC, "C" );
    
  mvd::MainWindow mainWindow;

  mainWindow.show();

  // Handle passing image filename from command-line
  if(argc>1)
    {
    try
      {
      // TODO: Replace with complex model (list of DatasetModel) when implemented.
      mvd::DatasetModel* model = mvd::Application::LoadDatasetModel(
        argv[1],
        // TODO: Remove width and height from dataset model loading.
        mainWindow.centralWidget()->width(),
        mainWindow.centralWidget()->height());
      
      mvd::Application::Instance()->SetModel( model );
      }
    catch( std::exception& exc )
      {
      // TODO: Report something usefull here
      }
    }
  
  return application.exec();
}

//
// Main functions implementations.
//
