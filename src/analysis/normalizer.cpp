// #include <iostream>
// #include <cmath>
// #include <map>
// #include <string>

// // Constants
// #define DEG_TO_RAD(degrees) ((degrees) * M_PI / 180.0)
// const double BOAT_LENGTH = 10.0; // Example value
// const double BOAT_WIDTH = 5.0;   // Example value

// struct Vec3
// {
//     double x, y, z;

//     Vec3(double xDepth, double yDepth, double zDepth) : x(xDepth), y(yDepth), z(zDepth) {}

//     // Rotate the vector around the Z-axis by the given angle (in degrees)
//     Vec3 rotateZ(double angleDegrees)
//     {
//         double rad = DEG_TO_RAD(angleDegrees);
//         double cosTheta = cos(rad);
//         double sinTheta = sin(rad);
//         return Vec3(x * cosTheta - y * sinTheta, x * sinTheta + y * cosTheta, z);
//     }

//     // Rotate the vector around the Y-axis by the given angle (in degrees)
//     Vec3 rotateY(double angleDegrees)
//     {
//         double rad = DEG_TO_RAD(angleDegrees);
//         double cosTheta = cos(rad);
//         double sinTheta = sin(rad);
//         return Vec3(x * cosTheta + z * sinTheta, y, -x * sinTheta + z * cosTheta);
//     }

//     // Rotate the vector around the X-axis by the given angle (in degrees)
//     Vec3 rotateX(double angleDegrees)
//     {
//         double rad = DEG_TO_RAD(angleDegrees);
//         double cosTheta = cos(rad);
//         double sinTheta = sin(rad);
//         return Vec3(x, y * cosTheta - z * sinTheta, y * sinTheta + z * cosTheta);
//     }
// };

// struct CameraNormalizer
// {

//     std::map<std::string, Vec3> cameraOffsets;

//     CameraNormalizer(const std::map<std::string, Vec3> &offsets) : cameraOffsets(offsets) {}

//     // Normalize the vector relative to the center of the boat
//     Vec3 normalizeToCenter(const std::string &cameraID)
//     {
//         double centerX = BOAT_LENGTH / 2.0;
//         double centerY = BOAT_WIDTH / 2.0;

//         auto it = cameraOffsets.find(cameraID);
//         if (it != cameraOffsets.end())
//         {
//             const Vec3 &offset = it->second;
//             return Vec3(offset.x - centerX, offset.y - centerY, offset.z);
//         }
//         else
//         {
//             // Handle the case where the camera ID is not found
//             std::cerr << "Camera ID not found: " << cameraID << std::endl;
//             return Vec3(0, 0, 0); // Default vector, can be adjusted as needed
//         }
//     }


//     Vec3 analyzedPosition() {

        
//     }

// };


// // Sequence Testing:
// // int main()
// // {
// //     std::map<std::string, Vec3> cameraOffsets = {
// //         {"camera1", Vec3(1.0, 2.0, 0.0)},
// //         {"camera2", Vec3(3.0, 4.0, 0.0)},
// //         // Add more camera offsets as needed
// //     };

// //     CameraNormalizer normalizer(cameraOffsets);
// //     Vec3 normalized = normalizer.normalizeToCenter("camera1");
// //     std::cout << "Normalized coordinates: (" << normalized.x << ", " << normalized.y << ", " << normalized.z << ")" << std::endl;

// //     return 0;
// // }