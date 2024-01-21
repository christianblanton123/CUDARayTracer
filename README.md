# CUDARayTracer
Based off of Peter Shirley's [Raytracing in one weekend](https://raytracing.github.io/) and Roger Allen's [Accelerated Ray Tracing in One Weekend in CUDA](https://developer.nvidia.com/blog/accelerated-ray-tracing-cuda/).

![alt text](https://github.com/christianblanton123/CUDARayTracer/blob/master/CUDARayTracer/500samplesperpixel.jpg?raw=true)

How to run:
- Fork or download repo
- On your local device, navigate to the destination of the repo
- Compile using `nvcc main.cu -o main_cuda`
- Run using `./main_cuda > out.ppm`  