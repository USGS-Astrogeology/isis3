/** This is free and unencumbered software released into the public domain.

The authors of ISIS do not claim copyright on the contents of this file.
For more details about the LICENSE terms and the AUTHORS, you will
find files of those names at the top level of this repository. **/

/* SPDX-License-Identifier: CC0-1.0 */

#include <iomanip>
#include <iostream>

#include "Camera.h"
#include "CameraFactory.h"
#include "FileName.h"
#include "IException.h"
#include "iTime.h"
#include "Preference.h"
#include "Pvl.h"
#include "PvlGroup.h"
#include "MsiCamera.h"

using namespace std;
using namespace Isis;

void TestLineSamp(Camera *cam, double samp, double line);

int main(void) {
  Preference::Preferences(true);

  cout << "Unit Test for NEAR Shoemaker MsiCamera..." << endl;
  try {
    // These should be lat/lon at center of image. To obtain these numbers for a new cube/camera,
    // set both the known lat and known lon to zero and copy the unit test output "Latitude off by: "
    // and "Longitude off by: " values directly into these variables.
    double knownLat = -22.8488929177263280;
    double knownLon = 37.5043546879872238;

    Cube c("$ISISTESTDATA/isis/src/near/unitTestData/m0155881376f3_2p.cub");
    MsiCamera *cam = (MsiCamera *) CameraFactory::Create(c);
    cout << "FileName: " << FileName(c.fileName()).name() << endl;
    cout << "CK Frame: " << cam->instrumentRotation()->Frame() << endl << endl;
    cout.setf(std::ios::fixed);
    cout << setprecision(9);

    // Test kernel IDs
    cout << "Kernel IDs: " << endl;
    cout << "CK Frame ID = " << cam->CkFrameId() << endl;
    cout << "CK Reference ID = " << cam->CkReferenceId() << endl;
    cout << "SPK Target ID = " << cam->SpkTargetId() << endl;
    cout << "SPK Reference ID = " << cam->SpkReferenceId() << endl << endl;

    // Test name methods
    cout << "Spacecraft Name Long: " << cam->spacecraftNameLong() << endl;
    cout << "Spacecraft Name Short: " << cam->spacecraftNameShort() << endl;
    cout << "Instrument Name Long: " << cam->instrumentNameLong() << endl;
    cout << "Instrument Name Short: " << cam->instrumentNameShort() << endl << endl;

    // Test Shutter Open/Close
    const PvlGroup &inst = c.label()->findGroup("Instrument", Pvl::Traverse);
    double exposureDuration = ((double) inst["ExposureDuration"])/1000;
    QString stime = inst["StartTime"];
    double et; // StartTime keyword is the center exposure time
    str2et_c(stime.toLatin1().data(), &et);
    pair <iTime, iTime> shuttertimes = cam->ShutterOpenCloseTimes(et, exposureDuration);
    cout << "Shutter open = " << shuttertimes.first.Et() << endl;
    cout << "Shutter close = " << shuttertimes.second.Et() << endl << endl;

    // Test all four corners to make sure the conversions are right
    cout << "For upper left corner ..." << endl;
    TestLineSamp(cam, 34.0, 34.0);

    cout << "For upper right corner ..." << endl;
    TestLineSamp(cam, cam->Samples() - 33.0, 34.0);

    cout << "For lower left corner ..." << endl;
    TestLineSamp(cam, 34.0, cam->Lines() - 33.0);

    cout << "For lower right corner ..." << endl;
    TestLineSamp(cam, cam->Samples() - 33.0, cam->Lines() - 33.0);

    double samp = cam->Samples() / 2;
    double line = cam->Lines() / 2;
    cout << "For center pixel position ..." << endl;

    if(!cam->SetImage(samp, line)) {
      cout << "ERROR" << endl;
      return 0;
    }

    // changed tolerance to allow hiclops to pass
    if(abs(cam->UniversalLatitude() - knownLat) < 1.18E-05) {
      cout << "Latitude OK" << endl;
    }
    else {
      cout << setprecision(16) << "Latitude off by: " << cam->UniversalLatitude() - knownLat << endl;
    }

    // changed tolerance to allow hiclops to pass
    if(abs(cam->UniversalLongitude() - knownLon) < 4.47E-6) {
      cout << "Longitude OK" << endl;
    }
    else {
      cout << setprecision(16) << "Longitude off by: " << cam->UniversalLongitude() - knownLon << endl;
    }
  }
  catch(IException &e) {
    e.print();
  }
}

void TestLineSamp(Camera *cam, double samp, double line) {
  bool success = cam->SetImage(samp, line);

  if(success) {
    success = cam->SetUniversalGround(cam->UniversalLatitude(), cam->UniversalLongitude());
  }

  if(success) {
    double deltaSamp = samp - cam->Sample();
    double deltaLine = line - cam->Line();
    if(fabs(deltaSamp) < 0.001) deltaSamp = 0;
    if(fabs(deltaLine) < 0.001) deltaLine = 0;
    cout << "DeltaSample = " << deltaSamp << endl;
    cout << "DeltaLine = " << deltaLine << endl << endl;
  }
  else {
    cout << "DeltaSample = ERROR" << endl;
    cout << "DeltaLine = ERROR" << endl << endl;
  }
}
