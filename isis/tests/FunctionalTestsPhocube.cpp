#include "phocube.h"

#include <QTemporaryFile>
#include <QTextStream>
#include <QStringList>
  
#include "BandManager.h"
#include "Fixtures.h"
#include "LineManager.h"
#include "PvlGroup.h"
#include "TestUtilities.h"

#include "gmock/gmock.h"

using namespace Isis;

static QString APP_XML = FileName("$ISISROOT/bin/xml/phocube.xml").expanded();

TEST_F(DefaultCube, FunctionalTestPhocubeDefault) {
  QString cubeFileName = tempDir.path() + "/phocubeTEMP.cub";
  QVector<QString> args = {"to=" + cubeFileName};
  UserInterface options(APP_XML, args);
  resizeCube(5, 5, 1);
  phocube(testCube, options);

  Cube cube(cubeFileName);
  Pvl *isisLabel = cube.label();

  // Only need to check that the bands were set correctly, do not need to check the image data
  ASSERT_EQ(cube.sampleCount(), testCube->sampleCount());
  ASSERT_EQ(cube.lineCount(), testCube->lineCount());
  ASSERT_EQ(cube.bandCount(), 5);

  PvlGroup bandBin = isisLabel->findGroup("BandBin", Pvl::Traverse);
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[0], "Phase Angle");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[1], "Emission Angle");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[2], "Incidence Angle");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[3], "Latitude");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[4], "Longitude");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("FilterName"), "CLEAR");
  EXPECT_EQ((int) bandBin.findKeyword("FilterId"), 4);

  for (int i = 0; i < cube.bandCount(); i++) {
    EXPECT_DOUBLE_EQ(bandBin.findKeyword("Center")[i].toDouble(), 1.0);
    EXPECT_DOUBLE_EQ(bandBin.findKeyword("Width")[i].toDouble(), 1.0);
  }

  cube.close();
}

TEST_F(DefaultCube, FunctionalTestPhocubeAllBands) {
  QString cubeFileName = tempDir.path() + "/phocubeTEMP.cub";
  QVector<QString> args = {"to=" + cubeFileName, "dn=true", "phase=true", "emission=true",
                           "incidence=true", "localemission=true", "localincidence=true",
                           "latitude=true", "longitude=true", "pixelresolution=true",
                           "lineresolution=true", "sampleresolution=true", "detectorresolution=true",
                           "obliquedetectorresolution=true", "northazimuth=true", "sunazimuth=true",
                           "spacecraftazimuth=true", "offnadirangle=true", "subspacecraftgroundazimuth=true",
                           "subsolargroundazimuth=true", "morphology=true", "albedo=true", "radec=true",
                           "bodyfixed=true", "localtime=true"};

  UserInterface options(APP_XML, args);
  resizeCube(5, 5, 1);
  phocube(testCube, options);

  Cube cube(cubeFileName);
  Pvl *isisLabel = cube.label();

  ASSERT_EQ(cube.sampleCount(), testCube->sampleCount());
  ASSERT_EQ(cube.lineCount(), testCube->lineCount());
  ASSERT_EQ(cube.bandCount(), 27);

  PvlGroup bandBin = isisLabel->findGroup("BandBin", Pvl::Traverse);
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[0], "CLEAR");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[1], "Phase Angle");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[2], "Emission Angle");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[3], "Incidence Angle");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[4], "Local Emission Angle");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[5], "Local Incidence Angle");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[6], "Latitude");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[7], "Longitude");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[8], "Pixel Resolution");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[9], "Line Resolution");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[10], "Sample Resolution");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[11], "Detector Resolution");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[12], "Oblique Detector Resolution");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[13], "North Azimuth");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[14], "Sun Azimuth");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[15], "Spacecraft Azimuth");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[16], "OffNadir Angle");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[17], "Sub Spacecraft Ground Azimuth");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[18], "Sub Solar Ground Azimuth");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[19], "Morphology Rank");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[20], "Albedo Rank");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[21], "Right Ascension");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[22], "Declination");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[23], "Body Fixed X");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[24], "Body Fixed Y");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[25], "Body Fixed Z");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[26], "Local Solar Time");
  cube.close();
}

TEST_F(DefaultCube, FunctionalTestPhocubeSpecialPixels) {
  QString cubeFileName = tempDir.path() + "/phocubeTEMP.cub";
  QVector<QString> args = {"to=" + cubeFileName, "specialpixels=false", "dn=true", "emission=false", 
                           "incidence=false", "latitude=false", "longitude=false"};
  UserInterface options(APP_XML, args);

  // Make the testing cube smaller and fill it with special pixels
  resizeCube(5, 5, 1);
  LineManager line(*testCube);
  int pixelValue = 1;
  int lineNum = 0;
  for(line.begin(); !line.end(); line++) {
    for(int i = 0; i < line.size(); i++) {
      if (lineNum == 0) {
        line[i] = (int) pixelValue++;
      }
      else if (lineNum == 1) {
        line[i] = LOW_REPR_SAT1;
      }
      else if (lineNum == 2) {
        line[i] = HIGH_REPR_SAT1;
      }
      else if (lineNum == 3) {
        line[i] = LOW_INSTR_SAT1;
      }
      else if (lineNum == 4) {
        line[i] = HIGH_INSTR_SAT1;
      }
      else {
        line[i] = NULL1;
      }
    }
    testCube->write(line);
    lineNum++;
  }

  phocube(testCube, options);

  Cube cube(cubeFileName);
  Pvl *isisLabel = cube.label();

  ASSERT_EQ(cube.sampleCount(), testCube->sampleCount());
  ASSERT_EQ(cube.lineCount(), testCube->lineCount());
  ASSERT_EQ(cube.bandCount(), 2);

  PvlGroup bandBin = isisLabel->findGroup("BandBin", Pvl::Traverse);
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[0], "CLEAR");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[1], "Phase Angle");

  int band = 1;
  LineManager outLine(cube);
  for (int i = 1; i <= cube.lineCount(); i++) { 
    outLine.SetLine(i, band);
    cube.read(outLine);
    for (int j = 0; j < outLine.size(); j++) { 
      if (i == 1) { // First line of both bands should match the input cube
        EXPECT_FALSE(IsSpecial(outLine[j]));
      }
      else if (band == 1) { // Rest of the first band are the special pixels from the input cube
        EXPECT_TRUE(IsSpecial(outLine[j]));
      }
      else {  // Rest of the second band should be all NULL
        EXPECT_EQ(outLine[j], NULL8);
      }
    }
    // Check next band
    if (band == 1 && i == cube.lineCount()) {
      i = 1;
      band++;
    }
  }

  cube.close();
}

// TEST_F(DefaultCube, FunctionalTestPhocubeOffBody) {
//   QString cubeFileName = "/Users/kdlee/Desktop/phocubeTEMP.cub";
//   QVector<QString> args = {"to=" + cubeFileName};
//   UserInterface options(APP_XML, args);
//   resizeCube(5, 5, 1);
//   phocube(testCube, options);

//   Cube cube(cubeFileName);
//   Pvl *isisLabel = cube.label();

//   ASSERT_EQ(cube.sampleCount(), testCube->sampleCount());
//   ASSERT_EQ(cube.lineCount(), testCube->lineCount());
//   ASSERT_EQ(cube.bandCount(), 5);

//   PvlGroup bandBin = isisLabel->findGroup("BandBin", Pvl::Traverse);
//   EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[0], "Phase Angle");
//   EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[1], "Emission Angle");
//   EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[2], "Incidence Angle");
//   EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[3], "Latitude");
//   EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[4], "Longitude");
//   EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("FilterName"), "CLEAR");
//   EXPECT_EQ((int) bandBin.findKeyword("FilterId"), 4);

//   for (int i = 0; i < cube.bandCount(); i++) {
//     EXPECT_DOUBLE_EQ(bandBin.findKeyword("Center")[i].toDouble(), 1.0);
//     EXPECT_DOUBLE_EQ(bandBin.findKeyword("Width")[i].toDouble(), 1.0);
//   }

//   cube.close();
// }

TEST_F(DefaultCube, FunctionalTestPhocubeMosaic) {
  QString cubeFileName = "/Users/kdlee/Desktop/phocubeTEMP.cub";
  QVector<QString> args = {"to=" + cubeFileName, "dn=true", "emission=false", 
                           "incidence=false", "latitude=false", "longitude=false"};
  UserInterface options(APP_XML, args);
  resizeCube(5, 5, 1);
  phocube(projTestCube, options);

  Cube cube(cubeFileName);
  Pvl *isisLabel = cube.label();

  ASSERT_EQ(cube.sampleCount(), testCube->sampleCount());
  ASSERT_EQ(cube.lineCount(), testCube->lineCount());
  ASSERT_EQ(cube.bandCount(), 5);

  PvlGroup bandBin = isisLabel->findGroup("BandBin", Pvl::Traverse);
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[0], "Phase Angle");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[1], "Emission Angle");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[2], "Incidence Angle");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[3], "Latitude");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("Name")[4], "Longitude");
  EXPECT_PRED_FORMAT2(AssertQStringsEqual, bandBin.findKeyword("FilterName"), "CLEAR");
  EXPECT_EQ((int) bandBin.findKeyword("FilterId"), 4);

  for (int i = 0; i < cube.bandCount(); i++) {
    EXPECT_DOUBLE_EQ(bandBin.findKeyword("Center")[i].toDouble(), 1.0);
    EXPECT_DOUBLE_EQ(bandBin.findKeyword("Width")[i].toDouble(), 1.0);
  }

  cube.close();
}