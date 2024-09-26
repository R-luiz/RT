#include "libs.hpp"

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

struct Light {
    Vector3 position;
    Vector3 intensity;
};

struct Sphere {
    Vector3 center;
    float radius;
    Vector3 color;
};

struct Scene {
    Camera camera;
    std::vector<Light> lights;
    std::vector<Sphere> spheres;
};

typedef struct s_vars {
    void *mlx;
    void *win;
    void *img;
    char *addr;
    int bits_per_pixel;
    int line_length;
    int endian;
} t_vars;

void my_mlx_pixel_put(t_vars *vars, int x, int y, int color)
{
    char *dst;

    dst = vars->addr + (y * vars->line_length + x * (vars->bits_per_pixel / 8));
    *(unsigned int*)dst = color;
}

Vector3 calculate_color(const Vector3& point, const Vector3& normal, const Sphere& sphere, const std::vector<Light>& lights)
{
    Vector3 color(0, 0, 0);
    Vector3 contribution(0, 0, 0);
    for (const auto& light : lights)
    {
        Vector3 light_dir = (light.position - point).normalize();
        float intensity = std::max(0.0f, normal * light_dir);
        contribution = Vector3( sphere.color.x() * light.intensity.x() * intensity,
                               sphere.color.y() * light.intensity.y() * intensity,
                               sphere.color.z() * light.intensity.z() * intensity);
        color = color + contribution;
    }
    return color * 255.0f;
}

bool intersect_sphere(const Vector3& ray_origin, const Vector3& ray_dir, const Sphere& sphere, float& t)
{
    Vector3 oc = ray_origin - sphere.center;
    float a = ray_dir * ray_dir;
    float b = 2.0 * (oc * ray_dir);
    float c = (oc * oc) - sphere.radius * sphere.radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return false;
    } else {
        float t0 = (-b - sqrt(discriminant)) / (2.0 * a);
        float t1 = (-b + sqrt(discriminant)) / (2.0 * a);
        t = (t0 < t1) ? t0 : t1;
        return t > 0;
    }
}

void render_scene(t_vars *vars, const Scene& scene)
{
    for (int y = 0; y < WINDOW_HEIGHT; ++y) {
        for (int x = 0; x < WINDOW_WIDTH; ++x) {
            float u = float(x) / float(WINDOW_WIDTH);
            float v = float(y) / float(WINDOW_HEIGHT);

            Vector3 ray_dir = scene.camera.ray_direction(u, v);
            float closest_t = 2147483647;
            const Sphere* closest_sphere = nullptr;

            for (const auto& sphere : scene.spheres) {
                float t;
                if (intersect_sphere(scene.camera.position, ray_dir, sphere, t) && t < closest_t) {
                    closest_t = t;
                    closest_sphere = &sphere;
                }
            }

            if (closest_sphere) {
                Vector3 hit_point = scene.camera.position + closest_t * ray_dir;
                Vector3 normal = (hit_point - closest_sphere->center).normalize();
                Vector3 color = calculate_color(hit_point, normal, *closest_sphere, scene.lights);
                int pixel_color = (static_cast<int>(color.x()) << 16) | (static_cast<int>(color.y()) << 8) | static_cast<int>(color.z());
                my_mlx_pixel_put(vars, x, y, pixel_color);
            } else {
                my_mlx_pixel_put(vars, x, y, 0);  // Background color (black)
            }
        }
    }
}

int key_hook(int keycode, t_vars *vars)
{
    if (keycode == 65307) // ESC key
    {
        mlx_destroy_window(vars->mlx, vars->win);
        mlx_destroy_display(vars->mlx);
        std::exit(0);
    }
    return (0);
}

Scene parse_scene_file(const std::string& filename)
{
    Scene scene;
    std::ifstream file(filename);
    std::string line;
    float x;
    float y;
    float z;
    float fov = 90;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        char type;
        iss >> type;

        if (type == 'C') {
            Vector3 position, direction, up;
            iss >> x >> y >> z;
            position = Vector3(x, y, z);
            iss >> x >> y >> z;
            direction = Vector3(x, y, z);
            iss >> x >> y >> z;
            up = Vector3(x, y, z);
            scene.camera = Camera(position, direction, up, fov, float(WINDOW_WIDTH) / float(WINDOW_HEIGHT));
            std::cout << "Camera added" << std::endl;
            std::cout << "Position: ";
            position.print();
            std::cout << "Direction: ";
            direction.print();
        } else if (type == 'L') {
            Light light;
            iss >> x >> y >> z;
            light.position = Vector3(x, y, z);
            iss >> x >> y >> z;
            light.intensity = Vector3(x, y, z);
            scene.lights.push_back(light);
            std::cout << "Light added" << std::endl;
            std::cout << "Position: ";
            light.position.print();
            std::cout << "Intensity: ";
            light.intensity.print();
        } else if (type == 'S') {
            Sphere sphere;
            iss >> x >> y >> z;
            sphere.center = Vector3(x, y, z);
            iss >> sphere.radius;
            iss >> x >> y >> z;
            sphere.color = Vector3(x, y, z);
            sphere.color = sphere.color / 255.0f;  // Normalize color values
            scene.spheres.push_back(sphere);
            std::cout << "Sphere added" << std::endl;
            std::cout << "Center: ";
            sphere.center.print();
            std::cout << "Color: ";
            std::cout << sphere.color.x() * 255 << " " << sphere.color.y() * 255 << " " << sphere.color.z() * 255 << std::endl;
        }
    }

    return scene;
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <scene_file>" << std::endl;
        return 1;
    }

    Scene scene = parse_scene_file(argv[1]);

    t_vars vars;

    vars.mlx = mlx_init();
    if (!vars.mlx)
        return (1);

    vars.win = mlx_new_window(vars.mlx, WINDOW_WIDTH, WINDOW_HEIGHT, (char *)"RT Scene Renderer");
    if (!vars.win)
    {
        mlx_destroy_display(vars.mlx);
        return (1);
    }

    vars.img = mlx_new_image(vars.mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    vars.addr = mlx_get_data_addr(vars.img, &vars.bits_per_pixel, &vars.line_length, &vars.endian);

    // Render the scene
    render_scene(&vars, scene);

    mlx_put_image_to_window(vars.mlx, vars.win, vars.img, 0, 0);

    mlx_key_hook(vars.win, reinterpret_cast<int (*)(int, void *)>(key_hook), &vars);

    mlx_loop(vars.mlx);

    return (0);
}