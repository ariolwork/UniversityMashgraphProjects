#version 420

#define float2 vec2
#define float3 vec3
#define float4 vec4
#define float4x4 mat4
#define float3x3 mat3

in float2 fragmentTexCoord;

layout(location = 0) out float4 fragColor;

uniform int g_screenWidth;
uniform int g_screenHeight; 

uniform float3 g_bBoxMin   = float3(-1,-1,-1);
uniform float3 g_bBoxMax   = float3(+1,+1,+1);

uniform float4x4 g_rayMatrix;

uniform float4   g_bgColor = float4(0,0,1,1);


//show parameters
uniform int show_type = 0;
uniform int fog = 0;
uniform int shadow_level = 0;

//constants
#define EPSILON 0.0001f
#define BULB_ITERATIONS    8
#define SPONGE_ITERATIONS  9
#define TRIAN_INTERATIONS 10
#define MAX_INTERATIONS 255

//help functions
float3 EyeRayDir(float x, float y, float w, float h)
{
	float fov = 3.141592654f/(2.0f); 
  float3 ray_dir;
  
	ray_dir.x = x+0.5f - (w/2.0f);
	ray_dir.y = y+0.5f - (h/2.0f);
	ray_dir.z = -(w)/tan(fov/2.0f);
	
  return normalize(ray_dir);
}

float length_n(float2 p, float n){
    return pow(pow(p.x, n)+pow(p.y, n), 1/n);
}

float2 min_by_x(float2 x, float2 y){
    if(x.x > y.x) return y;
    return x;
}
float smin(float x, float y, float k){
    float h = clamp(0.5 + 0.5 * (y - x) / k, 0.0, 1.0);
    return mix(y, x, h) - k * h * (1.0 - h);
}


//materials
struct material {
    float3 ambient;
    float3 diffuse;
    float3 specular;
    float alpha;

};
//0
material emerald = material(float3(0.0215, 0.1745, 0.0215), float3(0.07568, 0.61424, 0.07568), float3(0.633, 0.727811, 0.633), 0.6);
//1
material jade = material(float3(0.135, 0.2225, 0.1575), float3(0.54, 0.89, 0.63), float3(0.316228, 0.316228, 0.316228), 0.1);
//2
material obsidian = material(float3(0.05375, 0.05, 0.06625), float3(0.18275, 0.17, 0.22525), float3(0.332741, 0.328634, 0.346435), 0.3);
//3
material pearl = material(float3(0.25, 0.20725, 0.20725), float3(1.0, 0.829, 0.829), float3(0.296648, 0.296648, 0.296648), 0.088);
//4
material ruby = material(float3(0.1745, 0.01175, 0.01175), float3(0.61424, 0.04136, 0.04136), float3(0.727811, 0.626959, 0.626959), 0.6);
//5
material turquoise = material(float3(0.1, 0.18725, 0.1745), float3(0.396, 0.74151 , 0.69102), float3(0.297254, 0.30829, 0.306678), 0.1);
//6
material brass = material(float3(0.329412, 0.223529, 0.027451), float3(0.780392, 0.568627, 0.113725), float3(0.992157, 0.941176, 0.807843), 0.21794872);
//7
material bronze = material(float3(0.2125, 0.1275, 0.054), float3(0.714, 0.4284, 0.18144), float3(0.393548, 0.271906, 0.166721), 0.2);
//8
material chrome = material(float3(0.25, 0.25, 0.25), float3(0.4, 0.4, 0.4), float3(0.774597, 0.774597, 0.774597), 0.6);
//9
material copper = material(float3(0.19125, 0.0735, 0.0225), float3(0.7038, 0.27048, 0.0828), float3(0.256777, 0.137622, 0.086014), 0.1);
//10
material gold = material(float3(0.24725, 0.1995, 0.0745), float3(0.75164, 0.60648, 0.22648), float3(0.628281, 0.555802, 0.366065), 0.4);
//11
material silver = material(float3(0.19225, 0.19225, 0.19225), float3(0.50754, 0.50754, 0.50754), float3(0.508273, 0.508273, 0.508273), 0.4);
//mas
material materials[12] = {emerald, jade, obsidian, pearl, ruby, turquoise, brass, bronze, chrome, copper, gold, silver};

//light
struct light {
    float3 pos;
    float3 intensity;
};
//1
light light1 = light(float3(2,12,2), float3(0.4, 0.4, 0.4));
//2
light light2 = light(float3(0,6,-3), float3(0.6, 0.6, 0.6));


//scene distances
float sphereSDF(float3 p, float r) {
    return length(p) - r;
}

float floorSDF(float3 p) {
    return p.y;
}

float octahedronSDF(float3 p, float s) {
    p = abs(p);
    float m = p.x + p.y + p.z - s;
	float3 q;
         if( 3.0*p.x < m ) q = p.xyz;
    else if( 3.0*p.y < m ) q = p.yzx;
    else if( 3.0*p.z < m ) q = p.zxy;
    else return m*0.57735027;
    float k = clamp(0.5*(q.z-q.y+s),0.0,s); 
    return length(float3(q.x,q.y-s+k,q.z-k)); 
}

float torus2SDF(float3 p, float2 t ) {
  float2 q = float2(length(p.xz)-t.x,p.y);
  return length_n(q, 8.0)-t.y;
}


float cylinderSDF( float3 p, float2 h) {
    float2 d = abs(float2(length(p.xz),p.y)) - h;
    return min(max(d.x,d.y),0.0) + length(max(d,0.0));
}

float pole_cilinderSDF(float3 p, float2 h){
    float d1 = cylinderSDF(p, h);
    float d2 = cylinderSDF(p, float2(h.x/1.5, h.y+0.02));
    return max(d1, -d2);
}

float figureSDF(float3 p) {
    float d1 = length(max(abs(p)-float3(1,0.5,1),0.0))-0.15;//round_box
    float d2 = sphereSDF(p, 1.0);
    float d3 = pole_cilinderSDF(p-float3(0.0, 1, 0.0), float2(0.2, 2.7));
    float d4 = torus2SDF(p - float3(0.0, 3, 0.0), float2(1.0, 0.2));
    return min(smin(smin(d1, d2, 0.05f), d3, 0.15f), d4);
}

float prismSDF(float3 p, float2 h ) {
    float3 q = abs(p);
    return max(q.z-h.y,max((q.x*0.866025+q.y*0.5),q.y)-h.x);
}

float torusSDF(float3 p, float2 t ) {
  p.xyz = p.xzy;
  float2 q = float2(length_n(p.xz, 8.0)-t.x,p.y);
  return length_n(q, 8.0)-t.y;
}


float mandelbulbSDF(float3 p) {
    p /= 1.192;
    p.xyz = p.xzy;
    float3 z = p, dz = float3(0.0, 0.0, 0.0);
    float dr = 1.0, power = 8.0, r, theta, phi;
    for (int i = 0; i < BULB_ITERATIONS; i++) {
        r = length(z);
        if (r > 2.0) break;
        float theta = atan(z.y/z.x);
        float phi = asin(z.z/r);
        dr = pow(r, power-1.0) * power * dr + 1.0;
        r = pow(r, power);
        theta = theta * power;
        phi = phi * power;
        z = r * float3(cos(theta) * cos(phi), cos(phi) * sin(theta), sin(phi)) + p;
    }
    return 0.5 * log(r) * r / dr;
}

float spongeSDF(float3 z) {
    for (int i = 0; i < SPONGE_ITERATIONS; i++) {
        z = abs(z);
        z.xy = ((z.x < z.y)?(z.yx):(z.xy));
        z.xz = ((z.x < z.z)?(z.zx):(z.xz));
        z.zy = ((z.y < z.z)?(z.yz):(z.zy));	 
        z = z * 3.0 - 2.0;
        z.z += ((z.z < -1.0)?(2.0):(0.0));
    }
    z = abs(z)-float3(1.0, 1.0, 1.0);
    return min(max(z.x, max(z.y, z.z)), 0.0) + length(max(z, 0.0)) 
                        * 0.6 * pow(3.0, -float(SPONGE_ITERATIONS)); 
}


float triangleSDF(float3 p) {
    float3 va = float3(0.0, 0.57735, 0.0), vb = float3(0.0, -1.0, 1.15470),
    vc = float3(1.0, -1.0, -0.57735), vd = float3(-1.0, -1.0, -0.57735), v;
    float a = 0.0, r = 1.0, s = 1.0, dr, d, t;
    for (int i = 0; i < TRIAN_INTERATIONS; i++) {
        d = dot(p-va,p-va);
        v=va; dr=d; t=0.0;
        d = dot(p-vb,p-vb);
        if(d<dr){v=vb; dr=d; t=1.0;}
        d = dot(p-vc,p-vc);
        if(d<dr){v=vc; dr=d; t=2.0;}
        d = dot(p-vd,p-vd);
        if(d<dr){v=vd; dr=d; t=3.0;}
        p = v + 2.0*(p - v);
        r*= 2.0;
        a = t + 4.0*a;
        s*= 4.0;
    }
    return (sqrt(dr)-1.0)/r;
}

//fractal scene
float2 sceneSDF2(float3 p) {
    if(show_type == 20)
        return float2(spongeSDF(p-float3(0.0, 2.0, 0.0)), 10.0);
    if(show_type == 21)
        return float2(mandelbulbSDF(p-float3(0.0, 2.0, 0.0)), 10.0);
    if(show_type == 22)
        return float2(triangleSDF(p-float3(0.0, 2.0, 0.0)), 10.0);
    if(show_type == 23)
        return float2(max(spongeSDF(p-float3(0.0, 2.0, 0.0)), mandelbulbSDF(p-float3(0.0, 2.0, 0.0))), 10.0);    
}   

//figure scene
float2 sceneSDF1(float3 p) {
    float2 answ = float2(sphereSDF(p - float3(-2, 1.0, 0.0), 1), 10.0);
    answ = min_by_x(answ, float2(octahedronSDF(p-float3(2, 1.0, 0.0) ,1.0), 11.0));
    answ = min_by_x(answ, float2(figureSDF(p - float3(0.0, 1.0, -3)), 8.0));
    answ = min_by_x(answ, float2(prismSDF(p - float3(-3.5, 1.0, -3), float2(1.0,1.0)), 2.0));
    answ = min_by_x(answ, float2(torusSDF(p - float3(3.5, 1.5, -3), float2(1.0,0.5)), 0.0));
    answ = min_by_x(answ, float2(torus2SDF(p - float3(0.0, 1.0, 2), float2(1.0, 0.4)), 5.0));
    answ = min_by_x(answ, float2(floorSDF(p), 4.0));
    return answ;
}


//scene
float2 sceneSDF(float3 p) {
    if (show_type == 0 || show_type == 1) return sceneSDF1(p);
    if (show_type >= 20 && show_type <= 23) return sceneSDF2(p);
}



float2 RayTrace( in float3 ray_pos, in float3 ray_dir, float tmax, float tmin) {
    float depth = 0.0;
    for (int i = 0; i < MAX_INTERATIONS; i++) {
        float2 dist = sceneSDF(ray_pos + depth * ray_dir);
        if (dist.x < EPSILON) return float2(depth, dist.y);
        depth += dist.x;
        if (depth >= tmax) break;
    }
    return float2(tmax, -1.0);
}


//light and shadow:)
float raymarch_shadow(float3 ray_pos, float3 ray_dir, float tmin, float tmax, float resolution) {
    float result = 1.0;
    float depth = 0.0;
    for (int i = 0; i < MAX_INTERATIONS; ++i)
    {
        float h = sceneSDF(ray_pos + ray_dir * depth).x;
        if (h < EPSILON) return 0.0;
        result = min(result, resolution * h / depth);
        depth += h;
        if (depth >= tmax) break;
    }
    return result;
}

float3 estimateNormal(float3 p) {
    return normalize(float3(
        sceneSDF(float3(p.x + EPSILON, p.y, p.z)).x - sceneSDF(float3(p.x - EPSILON, p.y, p.z)).x,
        sceneSDF(float3(p.x, p.y + EPSILON, p.z)).x - sceneSDF(float3(p.x, p.y - EPSILON, p.z)).x,
        sceneSDF(float3(p.x, p.y, p.z  + EPSILON)).x - sceneSDF(float3(p.x, p.y, p.z - EPSILON)).x
    ));
}

float3 phongContribForLight(material m, float3 p, float3 ray_pos, light l, float tmin, float tmax) {
    float3 N = estimateNormal(p);
    float3 L = normalize(l.pos - p);
    float3 V = normalize(ray_pos - p);
    float3 R = normalize(reflect(-L, N));
    
    float dotLN = dot(L, N);
    float dotRV = dot(R, V);
    float3 lightIntensity = l.intensity;
    if (shadow_level != 0){
        float shadow = raymarch_shadow(l.pos, -L, 0.0, 0.9 * distance(l.pos, p), 20.0*shadow_level);
        lightIntensity *= shadow;    
    }
    if (dotLN < 0.0) return float3(0.0, 0.0, 0.0);  
    if (dotRV < 0.0) return lightIntensity * (m.diffuse * dotLN);
   
    return lightIntensity * (m.diffuse * dotLN + m.specular * pow(dotRV, m.alpha));
}
float3 phongIllumination(material m, float3 p, float3 ray_pos, float tmin, float tmax) { 
    float3 environment_light = 0.5 * float3(1.0, 1.0, 1.0);
    float3 color = environment_light * m.ambient;
    
    color += phongContribForLight(m, p, ray_pos, light1, tmin, tmax);    
    color += phongContribForLight(m, p, ray_pos, light2, tmin, tmax);    
    return color;
}

float3 shadow(float3 color, float p) {
    if (fog != 0) return color * exp( -0.000055*fog*p*p*p);
    return color;
}


void main(void)
{	
    //from example
    float w = float(g_screenWidth);
    float h = float(g_screenHeight);
    float x = fragmentTexCoord.x*w; 
    float y = fragmentTexCoord.y*h;
    float3 ray_pos = float3(0,0,0); 
    float3 ray_dir = EyeRayDir(x,y,w,h);
    ray_pos = (g_rayMatrix*float4(ray_pos,1)).xyz;
    ray_dir = float3x3(g_rayMatrix)*ray_dir;
    float tmax = 250;
    float tmin = 0.001;
    
    float2 dist = RayTrace(ray_pos, ray_dir, tmax, tmin);
    if (dist.x > tmax - EPSILON) {
        fragColor = g_bgColor;
    } else {
        float3 c = phongIllumination(materials[int(dist.y)], ray_pos + ray_dir * dist.x, ray_pos, tmin, tmax);
        fragColor = float4(shadow(c, dist.x), 1.0);
    }
}
