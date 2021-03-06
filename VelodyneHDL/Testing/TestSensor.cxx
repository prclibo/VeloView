// Copyright 2013 Velodyne Acoustics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#include "vtkPacketFileReader.h"
#include "vtkVelodyneHDLSource.h"
#include "vtkVelodyneHDLReader.h"
#include "vvPacketSender.h"

#include <vtkDataArray.h>
#include <vtkPointData.h>
#include <vtkTimerLog.h>
#include <vtkNew.h>

#include <map>
#include <string>
#include <cmath>
#include <cstdio>

#undef NDEBUG
#include <cassert>

//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <packet file>" << std::endl;
    return 1;
  }
  std::string filename(argv[1]);

  vtkNew<vtkVelodyneHDLSource> source;
  source->Start();

  try
    {
    std::string destinationIp = "127.0.0.1";
    int dataPort = 2368;
    vvPacketSender sender(filename, destinationIp, dataPort);
    //socket.connect(destinationEndpoint);

    for(size_t i = 0; i < 500; ++i)
      {
      sender.pumpPacket();
      }
    source->Update();
    vtkPolyData* result = source->GetOutput();

    while(!sender.done())
      {
      sender.pumpPacket();
      }

    source->Stop();
    }
  catch( std::exception& e )
    {
    std::cout << "Caught Exception: " << e.what() << std::endl;
    return 1;
    }
  return 0;
}
