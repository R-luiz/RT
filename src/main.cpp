#include "../libs/libs.hpp"

int main() {
    Vector3 v1(1, 2, 3);
    Vector3 v2(4, 5, 6);
    Vector3 v3 = v1 + v2;
    Vector3 v4 = v1 - v2;
    Vector3 v5 = v1 * 2;
    Vector3 v6 = v1 / 2;
    float dot = v1 * v2;
    Vector3 cross = v1.cross(v2);
    float len = v1.length();
    Vector3 norm = v1.normalize();

    std::cout << "v1: " << v1.x() << ", " << v1.y() << ", " << v1.z() << std::endl;
    std::cout << "v2: " << v2.x() << ", " << v2.y() << ", " << v2.z() << std::endl;
    std::cout << "v1 + v2: " << v3.x() << ", " << v3.y() << ", " << v3.z() << std::endl;
    std::cout << "v1 - v2: " << v4.x() << ", " << v4.y() << ", " << v4.z() << std::endl;
    std::cout << "v1 * 2: " << v5.x() << ", " << v5.y() << ", " << v5.z() << std::endl;
    std::cout << "v1 / 2: " << v6.x() << ", " << v6.y() << ", " << v6.z() << std::endl;
    std::cout << "v1 * v2: " << dot << std::endl;
    std::cout << "v1 x v2: " << cross.x() << ", " << cross.y() << ", " << cross.z() << std::endl;
    std::cout << "v1 length: " << len << std::endl;
    std::cout << "v1 normalized: " << norm.x() << ", " << norm.y() << ", " << norm.z() << std::endl;
    
    return 0;
}