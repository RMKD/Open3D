// ----------------------------------------------------------------------------
// -                        Open3D: www.open-3d.org                            -
// ----------------------------------------------------------------------------
// Imported on Apr 11th, 2018 from the Open3D project
// Copyright (c) 2018 www.open3d.org
// released under MIT licence
//
// A fork of their project is avilable at legacy-code.open-3d.org
// Refer to http://legacy-code.open-3d.org/blob/master/LICENSE
// for their full licence
// Check http://legacy-code.open-3d.org/graphs/contributors
// to get the list of their contributors and their contributions
//
// The rights of any later updates or modifications are reserved
// ----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2018, Hamdi Sahloul - www.open-3d.org
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#include <iostream>
#include <memory>
#include <Eigen/Dense>

#include <Open3D/Core/Core.h>
#include <Open3D/IO/IO.h>
#include <Open3D/Visualization/Visualization.h>

int main(int argc, char *argv[])
{
    using namespace open3d;
    SetVerbosityLevel(VerbosityLevel::VerboseAlways);

    if (argc != 3) {
        PrintInfo("> TestCameraPoseTrajectory trajectory_file pcds_dir\n");
        return 0;
    }
    const int NUM_OF_COLOR_PALETTE = 5;
    Eigen::Vector3d color_palette[NUM_OF_COLOR_PALETTE] = {
        Eigen::Vector3d(255, 180, 0) / 255.0,
        Eigen::Vector3d(0, 166, 237) / 255.0,
        Eigen::Vector3d(246, 81, 29) / 255.0,
        Eigen::Vector3d(127, 184, 0) / 255.0,
        Eigen::Vector3d(13, 44, 84) / 255.0,
    };

    PinholeCameraTrajectory trajectory;
    ReadPinholeCameraTrajectory(argv[1], trajectory);
    std::vector<std::shared_ptr<const Geometry>> pcds;
    for (size_t i = 0; i < trajectory.extrinsic_.size(); i++) {
        char buff[DEFAULT_IO_BUFFER_SIZE];
        sprintf(buff, "%scloud_bin_%d.pcd", argv[2], (int)i);
        if (filesystem::FileExists(buff)) {
            auto pcd = CreatePointCloudFromFile(buff);
            pcd->Transform(trajectory.extrinsic_[i]);
            pcd->colors_.clear();
            if ((int)i < NUM_OF_COLOR_PALETTE) {
                pcd->colors_.resize(pcd->points_.size(),
                        color_palette[i]);
            } else {
                pcd->colors_.resize(pcd->points_.size(),
                        (Eigen::Vector3d::Random() +
                        Eigen::Vector3d::Constant(1.0)) * 0.5);
            }
            pcds.push_back(pcd);
        }
    }
    DrawGeometriesWithCustomAnimation(pcds);

    return 0;
}
