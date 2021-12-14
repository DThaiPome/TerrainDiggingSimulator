#ifndef MARCHING_CUBES_HPP
#define MARCHING_CUBES_HPP

#include "Object.hpp"
#include <vector>
#include <map>
#include <unistd.h>

#define USE_THREADS
#define NUM_THREADS 12

#ifdef USE_THREADS
#include <pthread.h>
#include <semaphore.h>
#include <windows.h>
#endif

class MarchingCubes : public Object { 
public:
    MarchingCubes(unsigned int xSegs, unsigned int ySegs, unsigned int zSegs, float xDim, float yDim, float zDim, float* data, float surfaceLevel);
    ~MarchingCubes();

    void Init(float xDim, float yDim, float zDim, float surfaceLevel);
    void SetData(float* data);

    float* Subtract(float* subtractData);
    float* SphereExplosionData(float radius, float noise, float originX, float originY, float originZ);
private:
    struct Vector3 {
        float x;
        float y;
        float z;

        Vector3 operator+(const Vector3 & other) const {
            return {x + other.x, y + other.y, z + other.z}; 
        }
    };

    struct IVector3 {
        unsigned int x;
        unsigned int y;
        unsigned int z;

        bool operator==(const IVector3& other) const {
            return x == other.x && y == other.y && z == other.z;
        }

        bool operator<(const IVector3& other) const {
            return x < other.x || (x == other.x && y < other.y) || (x == other.x && y == other.y && z < other.z);
        }
    };

    struct Vertex {
        Vector3 position;
        Vector3 normal;
        unsigned int usageCount;
        unsigned int index;
    };

    struct MeshData {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
    };

    struct MarchArgs {
        MarchingCubes* obj;
        unsigned int x;
        unsigned int y;
        unsigned int z;
        unsigned int xSegs; 
        unsigned int ySegs; 
        unsigned int zSegs; 
        float xUnit; 
        float yUnit; 
        float zUnit; 
        float* data;
        float surfaceLevel;
        std::map<std::pair<IVector3, IVector3>, Vertex>* vertexMap;
        std::vector<unsigned int>* indices;
        unsigned int* globalIndex;
    };

    struct ThreadObj {
        pthread_t m_tid;
        pthread_barrier_t* m_barrierCopy;
        sem_t m_semaphore;
        std::vector<MarchArgs> m_queue;

        void AddJob(MarchArgs ma) {
            m_queue.push_back(ma);
        }

        void Init(pthread_barrier_t* barrier) {
            sem_init(&m_semaphore, 0, 0);
            m_barrierCopy = barrier;
            pthread_create(&m_tid, NULL, DoWork, (void*)this);
        }

        void Start() {
            sem_post(&m_semaphore);
        }

        static void* DoWork(void * self) {
            ThreadObj* threadObj = (ThreadObj*)self;
            while(true) {
                sem_wait(&(threadObj->m_semaphore));
                for(size_t i = 0; i < (threadObj->m_queue).size(); i++) {
                    MarchArgs ma = (threadObj->m_queue)[i];
                    ma.obj->start_cube_thread(&ma);
                }
                threadObj->m_queue.clear();

                pthread_barrier_wait(threadObj->m_barrierCopy);
            }
        }
    };

    MeshData march_cubes(unsigned int xSegs, unsigned int ySegs, unsigned int zSegs, float xUnit, float yUnit, float zUnit, float* data, float surfaceLevel);

    void fill_triangulations(const std::vector<std::pair<IVector3, IVector3>> & triangulation, std::map<std::pair<IVector3, IVector3>, MarchingCubes::Vertex> & vertexMap, std::vector<unsigned int> & indices, float* data, float surfaceLevel, float xSegs, float ySegs, float zSegs, float xUnit, float yUnit, float zUnit, unsigned int & globalIndex);

    Vector3 point_pair_to_position(std::pair<IVector3, IVector3> positionPair, Vector3 origin, float valueA, float valueB, float surfaceLevel, float xUnit, float yUnit, float zUnit);

    Vector3 triangle_normal(Vector3* positions);

    inline Vector3 cross_product(Vector3 vA, Vector3 vB);

    inline Vector3 get_vector(Vector3 vA, Vector3 vB);

    inline float dot(Vector3 vA, Vector3 vB);

    void start_cube_thread(MarchArgs* mArgs) {
        cube_thread(mArgs->x, mArgs->y, mArgs->z, mArgs->xSegs, mArgs->ySegs, mArgs->zSegs, mArgs->xUnit, mArgs->yUnit, mArgs->zUnit, mArgs->data, mArgs->surfaceLevel, *(mArgs->vertexMap), *(mArgs->indices), *(mArgs->globalIndex));
    }

    void cube_thread(unsigned int x, unsigned int y, unsigned int z, unsigned int xSegs, unsigned int ySegs, unsigned int zSegs, float xUnit, float yUnit, float zUnit, float* data, float surfaceLevel, std::map<std::pair<IVector3, IVector3>, Vertex> & vertexMap, std::vector<unsigned int> & indices, unsigned int & globalIndex);

    #ifdef USE_THREADS
    pthread_mutex_t m_dataLock;
    pthread_barrier_t m_barrier;
    #endif

    void lockData() {
        #ifdef USE_THREADS
        pthread_mutex_lock(&m_dataLock);
        #endif
    }

    void unlockData() {
        #ifdef USE_THREADS
        pthread_mutex_unlock(&m_dataLock);
        #endif
    }

    float* m_data;
    unsigned int m_xSegs;
    unsigned int m_ySegs;
    unsigned int m_zSegs;
    ThreadObj threads[NUM_THREADS];
};
#endif