#include "Fixtures.h"
#include "Pvl.h"
#include "PvlGroup.h"
#include "TestUtilities.h"

#include "isis2pds.h"

#include "gtest/gtest.h"

using namespace Isis;

static QString APP_XML = FileName("$ISISROOT/bin/xml/isis2pds.xml").expanded();

TEST_F(SmallCube, FunctionalTestIsis2pdsDefault) {
  QTemporaryDir prefix;
  QString outputFile = prefix.path() + "/outTemp.img";
  QVector<QString> args = {"from=" + testCube->fileName(),
                          "to=" + outputFile,
                          "checksum=yes"
                          };

  UserInterface options(APP_XML, args);
  try {
    isis2pds(options);
  }
  catch (IException &e) {
    FAIL() << "Unable to open image: " << e.what() << std::endl;
  }

  Pvl outputLabel(options.GetFileName("TO"));
  PvlObject imageObject = outputLabel.findObject("IMAGE");

  EXPECT_EQ(QString(outputLabel["PDS_VERSION_ID"]), "PDS3");
  EXPECT_EQ(QString(outputLabel["RECORD_TYPE"]), "UNDEFINED");
  EXPECT_EQ(int(outputLabel["LABEL_RECORDS"]), 806);
  EXPECT_EQ(int(outputLabel["^IMAGE"]), 807);
  EXPECT_EQ(QString(outputLabel["CHECKSUM"]), "ede3d3b685b4e137ba4cb2521329a75e");

  EXPECT_EQ(int(imageObject["LINES"]), 10);
  EXPECT_EQ(int(imageObject["LINE_SAMPLES"]), 10);
  EXPECT_EQ(int(imageObject["BANDS"]), 10);
  EXPECT_EQ(QString(imageObject["BAND_STORAGE_TYPE"]), "BAND_SEQUENTIAL");
  EXPECT_DOUBLE_EQ(double(imageObject["OFFSET"]), 0.0);
  EXPECT_DOUBLE_EQ(double(imageObject["SCALING_FACTOR"]), 1.0);
  EXPECT_EQ(int(imageObject["SAMPLE_BITS"]), 32);
  EXPECT_EQ(QString(imageObject["SAMPLE_BIT_MASK"]), "2#11111111111111111111111111111111#");
  EXPECT_EQ(QString(imageObject["SAMPLE_TYPE"]), "PC_REAL");
  EXPECT_EQ(QString(imageObject["CORE_NULL"]), "16#FF7FFFFB#");
  EXPECT_EQ(QString(imageObject["CORE_LOW_REPR_SATURATION"]), "16#FF7FFFFC#");
  EXPECT_EQ(QString(imageObject["CORE_LOW_INSTR_SATURATION"]), "16#FF7FFFFD#");
  EXPECT_EQ(QString(imageObject["CORE_HIGH_REPR_SATURATION"]), "16#FF7FFFFF#");
  EXPECT_EQ(QString(imageObject["CORE_HIGH_INSTR_SATURATION"]), "16#FF7FFFFE#");


}

TEST_F(SmallCube, FunctionalTestIsis2pdsDefaultMSB) {
  QTemporaryDir prefix;
  QString outputFile = prefix.path() + "/outTemp.img";
  QVector<QString> args = {"from=" + testCube->fileName(),
                          "to=" + outputFile,
                          "endian=msb",
                          "bittype=8bit"
                          };

  UserInterface options(APP_XML, args);
  try {
    isis2pds(options);
  }
  catch (IException &e) {
    FAIL() << "Unable to open image: " << e.what() << std::endl;
  }

  Pvl outputLabel(options.GetFileName("TO"));
  PvlObject imageObject = outputLabel.findObject("IMAGE");

  EXPECT_EQ(int(outputLabel["LABEL_RECORDS"]), 692);
  EXPECT_EQ(int(outputLabel["^IMAGE"]), 693);

  EXPECT_EQ(int(imageObject["SAMPLE_BITS"]), 8);
  EXPECT_EQ(QString(imageObject["SAMPLE_BIT_MASK"]), "2#11111111#");
  EXPECT_EQ(QString(imageObject["SAMPLE_TYPE"]), "MSB_UNSIGNED_INTEGER");
  EXPECT_EQ(int(imageObject["CORE_NULL"]), 0);
  EXPECT_EQ(int(imageObject["CORE_LOW_REPR_SATURATION"]), 1);
  EXPECT_EQ(int(imageObject["CORE_LOW_INSTR_SATURATION"]), 1);
  EXPECT_EQ(int(imageObject["CORE_HIGH_REPR_SATURATION"]), 255);
  EXPECT_EQ(int(imageObject["CORE_HIGH_INSTR_SATURATION"]), 255);

}

TEST_F(SmallCube, FunctionalTestIsis2pdsLinear8bit) {
  QTemporaryDir prefix;
  QString outputFile = prefix.path() + "/outTemp.img";
  QVector<QString> args = {"from=" + testCube->fileName(),
                          "to=" + outputFile,
                          "stretch=linear",
                          "bittype=8bit",
                          "lrs=yes",
                          "lis=yes",
                          "his=yes",
                          "hrs=yes",
                          "minpercent=5.0",
                          "maxpercent=45.0"
                          };

  UserInterface options(APP_XML, args);
  try {
    isis2pds(options);
  }
  catch (IException &e) {
    FAIL() << "Unable to open image: " << e.what() << std::endl;
  }

  Pvl outputLabel(options.GetFileName("TO"));
  PvlObject imageObject = outputLabel.findObject("IMAGE");

  EXPECT_EQ(int(outputLabel["LABEL_RECORDS"]), 697);
  EXPECT_EQ(int(outputLabel["^IMAGE"]), 698);

  EXPECT_EQ(int(imageObject["SAMPLE_BITS"]), 8);
  EXPECT_DOUBLE_EQ(double(imageObject["OFFSET"]), -0.012);
  EXPECT_DOUBLE_EQ(double(imageObject["SCALING_FACTOR"]), 0.004);
  EXPECT_EQ(int(imageObject["CORE_LOW_REPR_SATURATION"]), 1);
  EXPECT_EQ(int(imageObject["CORE_LOW_INSTR_SATURATION"]), 2);
  EXPECT_EQ(int(imageObject["CORE_HIGH_REPR_SATURATION"]), 255);
  EXPECT_EQ(int(imageObject["CORE_HIGH_INSTR_SATURATION"]), 254);

}

TEST_F(SmallCube, FunctionalTestIsis2pdsLinears16bit) {
  QTemporaryDir prefix;
  QString outputFile = prefix.path() + "/outTemp.img";
  QVector<QString> args = {"from=" + testCube->fileName(),
                          "to=" + outputFile,
                          "stretch=linear",
                          "bittype=s16bit",
                          "lrs=yes",
                          "lis=yes",
                          "his=yes",
                          "hrs=yes",
                          "minpercent=45.0",
                          "maxpercent=55.0"
                          };

  UserInterface options(APP_XML, args);
  try {
    isis2pds(options);
  }
  catch (IException &e) {
    FAIL() << "Unable to open image: " << e.what() << std::endl;
  }

  Pvl outputLabel(options.GetFileName("TO"));
  PvlObject imageObject = outputLabel.findObject("IMAGE");

  EXPECT_EQ(int(outputLabel["LABEL_RECORDS"]), 743);
  EXPECT_EQ(int(outputLabel["^IMAGE"]), 744);

  EXPECT_DOUBLE_EQ(double(imageObject["OFFSET"]), 0.49996947962765);
  EXPECT_DOUBLE_EQ(double(imageObject["SCALING_FACTOR"]), 1.52601861742713e-05);
  EXPECT_EQ(int(imageObject["SAMPLE_BITS"]), 16);
  EXPECT_EQ(QString(imageObject["SAMPLE_BIT_MASK"]), "2#1111111111111111#");
  EXPECT_EQ(QString(imageObject["SAMPLE_TYPE"]), "LSB_INTEGER");
  EXPECT_EQ(int(imageObject["CORE_LOW_REPR_SATURATION"]), -32767);
  EXPECT_EQ(int(imageObject["CORE_LOW_INSTR_SATURATION"]), -32766);
  EXPECT_EQ(int(imageObject["CORE_HIGH_REPR_SATURATION"]), -32764);
  EXPECT_EQ(int(imageObject["CORE_HIGH_INSTR_SATURATION"]), -32765);

}

TEST_F(SmallCube, FunctionalTestIsis2pdsLinearu16bit) {
  QTemporaryDir prefix;
  QString outputFile = prefix.path() + "/outTemp.img";
  QVector<QString> args = {"from=" + testCube->fileName(),
                          "to=" + outputFile,
                          "stretch=linear",
                          "bittype=u16bit",
                          "lrs=yes",
                          "lis=yes",
                          "his=yes",
                          "hrs=yes",
                          "minpercent=1.0",
                          "maxpercent=99.0"
                          };

  UserInterface options(APP_XML, args);
  try {
    isis2pds(options);
  }
  catch (IException &e) {
    FAIL() << "Unable to open image: " << e.what() << std::endl;
  }

  Pvl outputLabel(options.GetFileName("TO"));
  PvlObject imageObject = outputLabel.findObject("IMAGE");

  EXPECT_EQ(int(outputLabel["LABEL_RECORDS"]), 739);
  EXPECT_EQ(int(outputLabel["^IMAGE"]), 740);

  EXPECT_DOUBLE_EQ(double(imageObject["OFFSET"]), -4.5780558522814e-05);
  EXPECT_DOUBLE_EQ(double(imageObject["SCALING_FACTOR"]), 1.5260186174271301e-05);
  EXPECT_EQ(QString(imageObject["SAMPLE_TYPE"]), "LSB_UNSIGNED_INTEGER");
  EXPECT_EQ(int(imageObject["CORE_LOW_REPR_SATURATION"]), 1);
  EXPECT_EQ(int(imageObject["CORE_LOW_INSTR_SATURATION"]), 2);
  EXPECT_EQ(int(imageObject["CORE_HIGH_REPR_SATURATION"]), 65535);
  EXPECT_EQ(int(imageObject["CORE_HIGH_INSTR_SATURATION"]), 65534);

}

TEST_F(SmallCube, FunctionalTestIsis2pdsManual8bit) {
  QTemporaryDir prefix;
  QString outputFile = prefix.path() + "/outTemp.img";
  QVector<QString> args = {"from=" + testCube->fileName(),
                          "to=" + outputFile,
                          "stretch=manual",
                          "bittype=8bit",
                          "minimum=0",
                          "maximum=255"
                          };

  UserInterface options(APP_XML, args);
  try {
    isis2pds(options);
  }
  catch (IException &e) {
    FAIL() << "Unable to open image: " << e.what() << std::endl;
  }

  Pvl outputLabel(options.GetFileName("TO"));
  PvlObject imageObject = outputLabel.findObject("IMAGE");

  EXPECT_EQ(int(outputLabel["LABEL_RECORDS"]), 715);
  EXPECT_EQ(int(outputLabel["^IMAGE"]), 716);

  EXPECT_EQ(int(imageObject["SAMPLE_BITS"]), 8);
  EXPECT_DOUBLE_EQ(double(imageObject["OFFSET"]), -1.003937007874);
  EXPECT_DOUBLE_EQ(double(imageObject["SCALING_FACTOR"]), 1.003937007874);
  EXPECT_EQ(int(imageObject["CORE_LOW_REPR_SATURATION"]), 1);
  EXPECT_EQ(int(imageObject["CORE_LOW_INSTR_SATURATION"]), 1);
  EXPECT_EQ(int(imageObject["CORE_HIGH_REPR_SATURATION"]), 255);
  EXPECT_EQ(int(imageObject["CORE_HIGH_INSTR_SATURATION"]), 255);

}

TEST_F(SmallCube, FunctionalTestIsis2pdsManuals16bit) {
  QTemporaryDir prefix;
  QString outputFile = prefix.path() + "/outTemp.img";
  QVector<QString> args = {"from=" + testCube->fileName(),
                          "to=" + outputFile,
                          "stretch=manual",
                          "bittype=s16bit",
                          "lrs=yes",
                          "lis=yes",
                          "his=yes",
                          "hrs=yes",
                          "minimum=-32768",
                          "maximum=32767"
                          };

  UserInterface options(APP_XML, args);
  try {
    isis2pds(options);
  }
  catch (IException &e) {
    FAIL() << "Unable to open image: " << e.what() << std::endl;
  }

  Pvl outputLabel(options.GetFileName("TO"));
  PvlObject imageObject = outputLabel.findObject("IMAGE");

  EXPECT_EQ(int(outputLabel["LABEL_RECORDS"]), 738);
  EXPECT_EQ(int(outputLabel["^IMAGE"]), 739);

  EXPECT_DOUBLE_EQ(double(imageObject["OFFSET"]), -2.5001526018641);
  EXPECT_DOUBLE_EQ(double(imageObject["SCALING_FACTOR"]), 1.0000763009309);
  EXPECT_EQ(int(imageObject["SAMPLE_BITS"]), 16);
  EXPECT_EQ(QString(imageObject["SAMPLE_BIT_MASK"]), "2#1111111111111111#");
  EXPECT_EQ(QString(imageObject["SAMPLE_TYPE"]), "LSB_INTEGER");
  EXPECT_EQ(int(imageObject["CORE_NULL"]), -32768);
  EXPECT_EQ(int(imageObject["CORE_LOW_REPR_SATURATION"]), -32767);
  EXPECT_EQ(int(imageObject["CORE_LOW_INSTR_SATURATION"]), -32766);
  EXPECT_EQ(int(imageObject["CORE_HIGH_REPR_SATURATION"]), -32764);
  EXPECT_EQ(int(imageObject["CORE_HIGH_INSTR_SATURATION"]), -32765);

}

TEST_F(SmallCube, FunctionalTestIsis2pdsManualu16bit) {
  QTemporaryDir prefix;
  QString outputFile = prefix.path() + "/outTemp.img";
  QVector<QString> args = {"from=" + testCube->fileName(),
                          "to=" + outputFile,
                          "stretch=manual",
                          "bittype=u16bit",
                          "lrs=yes",
                          "lis=yes",
                          "his=yes",
                          "hrs=yes",
                          "minimum=0",
                          "maximum=65535"
                          };

  UserInterface options(APP_XML, args);
  try {
    isis2pds(options);
  }
  catch (IException &e) {
    FAIL() << "Unable to open image: " << e.what() << std::endl;
  }

  Pvl outputLabel(options.GetFileName("TO"));
  PvlObject imageObject = outputLabel.findObject("IMAGE");

  EXPECT_EQ(int(outputLabel["LABEL_RECORDS"]), 730);
  EXPECT_EQ(int(outputLabel["^IMAGE"]), 731);

  EXPECT_DOUBLE_EQ(double(imageObject["OFFSET"]), -3.0002289027926);
  EXPECT_DOUBLE_EQ(double(imageObject["SCALING_FACTOR"]), 1.0000763009309);
  EXPECT_EQ(QString(imageObject["SAMPLE_TYPE"]), "LSB_UNSIGNED_INTEGER");
  EXPECT_EQ(int(imageObject["CORE_NULL"]), 0);
  EXPECT_EQ(int(imageObject["CORE_LOW_REPR_SATURATION"]), 1);
  EXPECT_EQ(int(imageObject["CORE_LOW_INSTR_SATURATION"]), 2);
  EXPECT_EQ(int(imageObject["CORE_HIGH_REPR_SATURATION"]), 65535);
  EXPECT_EQ(int(imageObject["CORE_HIGH_INSTR_SATURATION"]), 65534);

}

TEST(isis2pdsTest, FunctionalTestIsis2pdsOffset) {
  QTemporaryDir prefix;
  QString outputFile = prefix.path() + "/outTemp.img";
  QVector<QString> args = {"from=data/isis2pds/ldem_4_reduced.cub",
                          "to=" + outputFile,
                          "checksum=yes"
                          };

  UserInterface options(APP_XML, args);
  try {
    isis2pds(options);
  }
  catch (IException &e) {
    FAIL() << "Unable to open image: " << e.what() << std::endl;
  }

  Pvl outputLabel(options.GetFileName("TO"));
  PvlObject imageObject = outputLabel.findObject("IMAGE");

  EXPECT_EQ(QString(outputLabel["PDS_VERSION_ID"]), "PDS3");
  EXPECT_EQ(QString(outputLabel["RECORD_TYPE"]), "UNDEFINED");
  EXPECT_EQ(int(outputLabel["LABEL_RECORDS"]), 2032);
  EXPECT_EQ(int(outputLabel["^IMAGE"]), 2033);
  EXPECT_EQ(QString(outputLabel["CHECKSUM"]), "315348326c41130fdefd0615c24a1ac9");
  EXPECT_EQ(QString(outputLabel["TARGET_NAME"]), "MOON");

  EXPECT_EQ(int(imageObject["LINES"]), 240);
  EXPECT_EQ(int(imageObject["LINE_SAMPLES"]), 480);
  EXPECT_EQ(int(imageObject["BANDS"]), 1);
  EXPECT_EQ(QString(imageObject["BAND_STORAGE_TYPE"]), "BAND_SEQUENTIAL");
  EXPECT_DOUBLE_EQ(double(imageObject["OFFSET"]), 0.0);
  EXPECT_DOUBLE_EQ(double(imageObject["SCALING_FACTOR"]), 1.0);
  EXPECT_EQ(int(imageObject["SAMPLE_BITS"]), 32);
  EXPECT_EQ(QString(imageObject["SAMPLE_BIT_MASK"]), "2#11111111111111111111111111111111#");
  EXPECT_EQ(QString(imageObject["SAMPLE_TYPE"]), "PC_REAL");
  EXPECT_EQ(QString(imageObject["CORE_NULL"]), "16#FF7FFFFB#");
  EXPECT_EQ(QString(imageObject["CORE_LOW_REPR_SATURATION"]), "16#FF7FFFFC#");
  EXPECT_EQ(QString(imageObject["CORE_LOW_INSTR_SATURATION"]), "16#FF7FFFFD#");
  EXPECT_EQ(QString(imageObject["CORE_HIGH_REPR_SATURATION"]), "16#FF7FFFFF#");
  EXPECT_EQ(QString(imageObject["CORE_HIGH_INSTR_SATURATION"]), "16#FF7FFFFE#");

  PvlObject projectionObject = outputLabel.findObject("IMAGE_MAP_PROJECTION");

  EXPECT_EQ(QString(projectionObject["MAP_PROJECTION_TYPE"]), "SIMPLE CYLINDRICAL");
  EXPECT_EQ(QString(projectionObject["PROJECTION_LATITUDE_TYPE"]), "PLANETOCENTRIC");
  EXPECT_DOUBLE_EQ(double(projectionObject["A_AXIS_RADIUS"]), 1737.4);
  EXPECT_DOUBLE_EQ(double(projectionObject["B_AXIS_RADIUS"]), 1737.4);
  EXPECT_DOUBLE_EQ(double(projectionObject["C_AXIS_RADIUS"]), 1737.4);
  EXPECT_EQ(QString(projectionObject["FIRST_STANDARD_PARALLEL"]), "N/A");
  EXPECT_EQ(QString(projectionObject["SECOND_STANDARD_PARALLEL"]), "N/A");
  EXPECT_EQ(QString(projectionObject["COORDINATE_SYSTEM_NAME"]), "PLANETOCENTRIC");
  EXPECT_EQ(QString(projectionObject["POSITIVE_LONGITUDE_DIRECTION"]), "EAST");
  EXPECT_EQ(QString(projectionObject["KEYWORD_LATITUDE_TYPE"]), "PLANETOCENTRIC");
  EXPECT_DOUBLE_EQ(double(projectionObject["CENTER_LATITUDE"]), 0.0);
  EXPECT_DOUBLE_EQ(double(projectionObject["CENTER_LONGITUDE"]), 180.0);
  EXPECT_EQ(int(projectionObject["LINE_FIRST_PIXEL"]), 1);
  EXPECT_EQ(int(projectionObject["LINE_LAST_PIXEL"]), 240);
  EXPECT_EQ(int(projectionObject["SAMPLE_FIRST_PIXEL"]), 1);
  EXPECT_EQ(int(projectionObject["SAMPLE_LAST_PIXEL"]), 480);
  EXPECT_DOUBLE_EQ(double(projectionObject["MAP_PROJECTION_ROTATION"]), 0.0);
  EXPECT_DOUBLE_EQ(double(projectionObject["MAP_RESOLUTION"]), 1.3333333333333);
  EXPECT_DOUBLE_EQ(double(projectionObject["MAP_SCALE"]), 22.74252);
  EXPECT_DOUBLE_EQ(double(projectionObject["MAXIMUM_LATITUDE"]), 90.0);
  EXPECT_DOUBLE_EQ(double(projectionObject["MINIMUM_LATITUDE"]), -90.0);
  EXPECT_DOUBLE_EQ(double(projectionObject["EASTERNMOST_LONGITUDE"]), 360.0);
  EXPECT_DOUBLE_EQ(double(projectionObject["WESTERNMOST_LONGITUDE"]), 0.0);
  EXPECT_DOUBLE_EQ(double(projectionObject["LINE_PROJECTION_OFFSET"]), 119.5);
  EXPECT_DOUBLE_EQ(double(projectionObject["SAMPLE_PROJECTION_OFFSET"]), 239.5);

}
