#include "libs.hpp"

class Camera
{
public:
    Vector3 position;
    Vector3 direction;
    Vector3 up;
    float fov;
    float aspect_ratio;

    Camera() : position(0, 0, 0), direction(0, 0, -1), up(0, 1, 0), fov(90), aspect_ratio(16.0f/9.0f) {}

    Camera(const Vector3& pos, const Vector3& dir, const Vector3& up, float fov, float aspect)
        : position(pos), direction(dir.normalize()), up(up.normalize()), fov(fov), aspect_ratio(aspect) {
        // Ensure up vector is perpendicular to direction
        this->up = this->direction.cross(this->up.cross(this->direction)).normalize();
    }

    Vector3 ray_direction(float u, float v) const {
        float fov_rad = fov * M_PI / 180.0;
        float half_height = tan(fov_rad / 2);
        float half_width = aspect_ratio * half_height;

        Vector3 w = -1 * direction;
        Vector3 u_vec = up.cross(w).normalize();
        Vector3 v_vec = w.cross(u_vec);

        Vector3 lower_left_corner = position - half_width * u_vec - half_height * v_vec - w;
        Vector3 horizontal = 2 * half_width * u_vec;
        Vector3 vertical = 2 * half_height * v_vec;

        return (lower_left_corner + u * horizontal + v * vertical - position).normalize();
    }
};