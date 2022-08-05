#version 130

#define PI 3.1415926535897932384626433832795
#define MANDELBROT_SET  1
#define JULIA_SET       1 - MANDELBROT_SET

uniform     vec2        u_resolution;
uniform     sampler2D   u_tex;
uniform     float       u_depth;
uniform     vec2        u_position;
uniform     float       u_Re;
uniform     float       u_Im;

void main() {
    double _scaleX = double(u_resolution.y / u_resolution.x);
    double _zx = double(2.0*(gl_FragCoord.x - u_resolution.x/2.0) + u_position.x);
    _zx /= -double(u_resolution.x)*_scaleX;
    _zx *= double(u_depth);
    double _zy = double(2.0*(gl_FragCoord.y - u_resolution.y/2.0) + u_position.y);
    _zy /= double(u_resolution.y);
    _zy *= double(u_depth);

    int _iteration = 0;
    const int _max_iteration = 100;
    const int _real_max_iteration = 1000;

    #if JULIA_SET
    double _cx = double(u_Re);
    double _cy = double(u_Im);
    double _sq = double(sqrt(float(_cx*_cx + _cy*_cy)));
    double _R = double(0.0);
    while((_R*_R-_R) < _sq) _R += double(0.5);
    _R *= double(1.05);
    _zx *= _R; // 0.6 - scale fractal
    _zy *= _R;
    while(((_zx*_zx + _zy*_zy) < _R*_R) && _iteration < _max_iteration)
    {
        double xtemp = _zx * _zx - _zy * _zy;
        _zy = double(2.0) * _zx * _zy + _cy;
        _zx = xtemp + _cx;
        _iteration++; 
    }
    #endif

    #if MANDELBROT_SET
    double x = double(0.0);
    double y = double(0.0);
    while((x*x + y*y <= double(4.0)) && _iteration < _max_iteration)
    {
        double xtemp = x * x - y * y - _zx;
        y = double(2.0) * x * y + _zy;
        x = xtemp;
        _iteration++; 
    }
    #endif

    _iteration *= _real_max_iteration/_max_iteration;
    float part = float(_iteration) / float(_max_iteration);
    vec4 gradient = texture(u_tex, vec2(part, 0));
    if(_iteration == _max_iteration)
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    else
        gl_FragColor = gradient;
}   