__constant float M_EPSILON = 0.00003f;
__constant int SAMPLES = 1024;
__constant int MAX_BOUNCES = 8;

__constant float3 BACKGROUND_COLOR =
  (float3)(0.15f, 0.15f, 0.25f);

typedef struct Ray
{
  float3 origin;
  float3 dir;
} Ray;

typedef struct Sphere
{
  float radius;
  float3 position;
  float3 color;
  float3 emission;
} Sphere;

static float get_random(unsigned int* seed_0,
                        unsigned int* seed_1)
{
  *seed_0 = 36969 * ((*seed_0) & 65535) + ((*seed_0) >> 16);
  *seed_1 = 18000 * ((*seed_1) & 65535) + ((*seed_1) >> 16);

  unsigned int i_res = ((*seed_0) << 16) + (*seed_1);

  union
  {
    float f;
    unsigned int ui;
  } res;

  res.ui = (i_res & 0x007fffff) | 0x40000000;

  return (res.f - 2.0f) / 2.0f;
}

float intersect_sphere(const struct Sphere* sphere,
                       const struct Ray* ray)
{
  float3 ray_to_center = sphere->position - ray->origin;

  float b = dot(ray_to_center, ray->dir);
  float c = dot(ray_to_center, ray_to_center) -
            sphere->radius * sphere->radius;
  float disc = b * b - c;

  if (disc < 0.0f) {
    return 0.0f;
  } else {
    disc = sqrt(disc);
  }

  if ((b - disc) > M_EPSILON) {
    return b - disc;
  }
  if ((b + disc) > M_EPSILON) {
    return b + disc;
  }

  return 0.0f;
}

bool intersect_scene(__constant Sphere* spheres,
                     const Ray* ray,
                     float* t,
                     int* sphere_id,
                     const int sphere_count)
{
  float inf = 1e20f;
  *t = inf;

  for (int i = 0; i < sphere_count; i++) {

    Sphere sphere = spheres[i];

    float hit_dist = intersect_sphere(&sphere, ray);

    if (hit_dist != 0.0f && hit_dist < *t) {
      *t = hit_dist;
      *sphere_id = i;
    }
  }

  return *t < inf;
}

float3 trace(__constant Sphere* spheres,
             const Ray* cam_ray,
             const int sphere_count,
             unsigned int* seed_0,
             unsigned int* seed_1)
{
  Ray ray = *cam_ray;

  float3 accum_color = (float3)(0.0f, 0.0f, 0.0f);
  float3 mask = (float3)(1.0f, 1.0f, 1.0f);

  for (int bounce = 0; bounce < MAX_BOUNCES; bounce++) {

    float t;        // distance to intersection
    int hit_id = 0; // index of intersected sphere

    // return background color if ray misses

    if (!intersect_scene(
          spheres, &ray, &t, &hit_id, sphere_count)) {
      return accum_color += mask * BACKGROUND_COLOR;
    }

    Sphere hit_sphere = spheres[hit_id];
    float3 hit_point = ray.origin + ray.dir * t;

    // surface normal

    float3 normal =
      normalize(hit_point - hit_sphere.position);
    float3 normal_facing = dot(normal, ray.dir) < 0.0f
                             ? normal
                             : normal * (-1.0f);

    // compute 2 random points within hemisphere of hit
    // point

    float rand_1 = 2.0f * M_PI * get_random(seed_0, seed_1);
    float rand_2 = get_random(seed_0, seed_1);
    float rand_2sq = sqrt(rand_2);

    // create a local orthogonal coordinate frame centered
    // at hit point

    float3 w = normal_facing;
    float3 axis = fabs(w.x) > 0.1f
                    ? (float3)(0.0f, 1.0f, 0.0f)
                    : (float3)(1.0f, 0.0f, 0.0f);
    float3 u = normalize(cross(axis, w));
    float3 v = cross(w, u);

    // use the coordinate frame and random numbers to
    // compute the next ray direction

    float3 new_dir = normalize(u * cos(rand_1) * rand_2sq +
                               v * sin(rand_1) * rand_2sq +
                               w * sqrt(1.0f - rand_2));

    // add small offset to prevent self-intersection

    ray.origin = hit_point + normal_facing * M_EPSILON;
    ray.dir = new_dir;

    // add the color and light contributions to the
    // accumulated color

    accum_color += mask * hit_sphere.emission;
    mask *= hit_sphere.color;

    // cosine-weighted importance sampling

    mask *= dot(new_dir, normal_facing);

  }

  return accum_color;
}

// calculate ray direction and position based on fov,
// aspect ratio, and dispatch position

// ref: https://tinyurl.com/824zvkvx

struct Ray create_camera_ray(const int x,
                             const int y,
                             const int w,
                             const int h)
{
  float fx = (float)x / (float)w;
  float fy = (float)y / (float)h;

  float aspect_r = (float)w / (float)h;
  float fx_2 = (fx - 0.5f) * aspect_r;
  float fy_2 = fy - 0.5f;

  float3 pixel_pos = (float3)(fx_2, -fy_2, 0.0f);

  struct Ray ray;
  ray.origin = (float3)(0.0f, 0.1f, 2.0f);
  ray.dir = normalize(pixel_pos - ray.origin);

  return ray;
}

// ==== main ====

__kernel void render_kernel(__constant Sphere* spheres,
                            const int width,
                            const int height,
                            const int sphere_count,
                            __global float3* output)
{
  const int work_item_id = get_global_id(0);

  int x_coord = work_item_id % width;
  int y_coord = work_item_id / width;

  unsigned int seed_0 = x_coord;
  unsigned int seed_1 = y_coord;

  struct Ray ray =
    create_camera_ray(x_coord, y_coord, width, height);

  float3 final_col = (float3)(0.0f, 0.0f, 0.0f);
  float inv_samples = 1.0f / SAMPLES;

  for (int i = 0; i < SAMPLES; i++) {
    final_col +=
      trace(spheres, &ray, sphere_count, &seed_0, &seed_1) *
        inv_samples;
  }

  output[work_item_id] = final_col;
}
