#include <SDL/SDL.h>
#include <SDL/SDL_events.h>
#include <math.h>
#include <vector>
#include "vector.h"
#include "util.h"
#include "sdl.h"
#include "color.h"
#include "camera.h"
#include "geometry.h"
#include "shading.h"
#include "environment.h"
#include "mesh.h"
#include "random_generator.h"
#include "scene.h"
#include "lights.h"
#include "cxxptl_sdl.h"
#include "chess_logic.h"
using std::vector;


Color vfb[VFB_MAX_SIZE][VFB_MAX_SIZE];
bool needsAA[VFB_MAX_SIZE][VFB_MAX_SIZE];
Vector positions[8][8];
GameBoard board;
bool visibilityCheck(const Vector& start, const Vector& end);
ThreadPool pool;

Color raytrace(const Ray& ray)
{
	if (ray.depth > scene.settings.maxTraceDepth) return Color(0, 0, 0);
	Node* closestNode = NULL;
	double closestDist = INF;
	IntersectionInfo closestInfo;
	for (auto& node: scene.nodes) {

        if(!node->isInGame) continue;

		IntersectionInfo info;

		if (!node->intersect(ray, info)) continue;

		if (info.distance < closestDist) {
			closestDist = info.distance;
			closestNode = node;
			closestInfo = info;
		}
	}

	// check if we hit the sky:
	if (closestNode == NULL) {
		if (scene.environment) return scene.environment->getEnvironment(ray.dir);
		else return Color(0, 0, 0);
	} else {
		return closestNode->shader->shade(ray, closestInfo);
	}
}


bool visibilityCheck(const Vector& start, const Vector& end)
{
	Ray ray;
	ray.start = start;
	ray.dir = end - start;
	ray.dir.normalize();

	double targetDist = (end - start).length();

	for (auto& node: scene.nodes) {
        if(node->isInGame) continue;

		IntersectionInfo info;

		if (!node->intersect(ray, info)) continue;

		if (info.distance < targetDist) {
			return false;
		}
	}
	return true;
}

void debugRayTrace(int x, int y)
{
	Ray ray = scene.camera->getScreenRay(x, y);
	ray.flags |= RF_DEBUG;
	raytrace(ray);
}

Color raytraceSinglePixel(double x, double y)
{
    Ray ray = scene.camera->getScreenRay(x, y);
    return raytrace(ray);
}

Color renderPixel(int x, int y)
{
    return raytraceSinglePixel(x, y);
}

class RenderScreenTask: public Parallel {
protected:
	const vector<Rect>& buckets;
	InterlockedInt counter;
public:
	RenderScreenTask(const vector<Rect>& buckets): buckets(buckets), counter(0) {}
};

// Main rendering task. For scenes, that have implicit AA (with DOF or GI), this
// is all that is needed. If explicit AA is needed, this task is just a prepass
// that renders the base screen (1-sample-per-pixel).
struct MainRenderTask: public RenderScreenTask {
	bool finalPass;
	MainRenderTask(const vector<Rect>& buckets): RenderScreenTask(buckets)
	{
		finalPass = !scene.settings.needAApass();
	}

	void entry(int threadIdx, int threadCount)
	{
		int i;
		while ((i = counter++) < int(buckets.size())) {
			const Rect& r = buckets[i];
			if (!scene.settings.interactive && finalPass && !markRegion(r)) return;
			for (int y = r.y0; y < r.y1; y++)
				for (int x = r.x0; x < r.x1; x++) {
					vfb[y][x] = renderPixel(x, y);
				}
			if (!scene.settings.interactive && !displayVFBRect(r, vfb)) return;
		}
	}
};

// If explicit AA is needed, this task refines the image around sharp edges
// (as detected by detectAApixels()).
struct RefineRenderTask: public RenderScreenTask {
	RefineRenderTask(const vector<Rect>& buckets): RenderScreenTask(buckets) {}

	void entry(int threadIdx, int threadCount)
	{
		const double kernel[5][2] = {
			// note that this sample is already rendered in vfb[][]:
			{ 0.0, 0.0 },

			// refinement adds these samples:
			{ 0.6, 0.0 },
			{ 0.0, 0.6 },
			{ 0.3, 0.3 },
			{ 0.6, 0.6 },
		};
		int i;
		while ((i = counter++) < int(buckets.size())) {
			const Rect& r = buckets[i];
			// see if the bucket contains anything interesting at all:
			bool skipBucket = true;
			for (int y = r.y0; y < r.y1 && skipBucket; y++)
				for (int x = r.x0; x < r.x1; x++)
					if (needsAA[y][x]) {
						skipBucket = false;
						break;
					}
			if (skipBucket) continue;

			if (!markRegion(r)) return;
			for (int y = r.y0; y < r.y1; y++)
				for (int x = r.x0; x < r.x1; x++) {
					if (!needsAA[y][x]) continue;
					Color& sum = vfb[y][x];
					for (int j = 1; j < COUNT_OF(kernel); j++)
						sum += raytraceSinglePixel(x + kernel[j][0], y + kernel[j][1]);
					sum /= COUNT_OF(kernel);
				}
			if (!scene.settings.interactive && !displayVFBRect(r, vfb)) return;
		}
	}
};

void render()
{
	scene.beginFrame();
	vector<Rect> buckets = getBucketsList();

	MainRenderTask mtrend(buckets);
	pool.run(&mtrend, scene.settings.numThreads);

}

int renderSceneThread(void* /*unused*/)
{
	render();
	rendering = false;
	return 0;
}

Node* getFigure(const Ray& ray)
{
    Node* closestNode = NULL;
	double closestDist = INF;
	IntersectionInfo closestInfo;
    IntersectionInfo info;

    for(int i = 1; i < (int)scene.nodes.size(); ++i)
    {
        if(!scene.nodes[i]->isInGame) continue;

        IntersectionInfo info;

        if(!scene.nodes[i]->intersect(ray, info)) continue;

        if (info.distance < closestDist)
        {
            closestDist = info.distance;
            closestNode = scene.nodes[i];
            closestInfo = info;
        }

    }

    return closestNode;
}

void getSquare(const Vector& point, int& x, int&y)
{
    x = (int) floor(point[0] + 200) / 50;
    y = (int) floor(point[2] + 200) / 50;
}
/*
void renderSquare(std::vector<int>& bounds)
{
    for(int i = 0; i < 480; ++i)
        for(int j = 0; j < 640; ++j)
            vfb[i][j] = renderPixel(j, i);

}

void getSquareRender(Node* node, std::vector<int>& bounds)
{
    Vector v1 = ((Mesh&)(*node->geom)).bbox.vmin;
    Vector v2 = ((Mesh&)(*node->geom)).bbox.vmax;
    v1 = node->transform.point(v1);
    v2 = node->transform.point(v2);
}*/

bool makeMove(Node* node, bool& running)
{
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_MOUSEBUTTONDOWN:
                {
                    int x, y;
                    SDL_GetMouseState(&x, &y);

                    Ray ray = scene.camera->getScreenRay(x, y);
                    IntersectionInfo info;
                    if(scene.nodes[0]->intersect(ray, info))
                    {
                        getSquare(info.ip, x, y);
                        if(board.CanMakeAMove(node->x + 1, node->y + 1, x + 1, y + 1));
                        {
                            node->transform.translate(positions[x][y]);
                            return true;
                        }

                        return false;
                    }
                   return false;
                }
                case SDL_QUIT:
                    running = false;
                    return false;
                case SDL_KEYDOWN:
                {
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            running = false;
                            return false;
                        default:
                            return false;
                    }
                    return false;
                }
            }
        }
    }
}

void mainloop(void)
{
	SDL_ShowCursor(1);
	bool running = true;
    render();

	while (running)
    {
		displayVFB(vfb);

		SDL_Event event;

		while (SDL_PollEvent(&event))
        {
			switch (event.type)
			{
                case SDL_MOUSEBUTTONDOWN:
                {
                    int x, y;
                    SDL_GetMouseState(&x, &y);

                    Ray ray = scene.camera->getScreenRay(x, y);
                    Node *figure = getFigure(ray);

                    if(!figure)
                        continue;

                    if(makeMove(figure, running))
                        render();

                    break;
                }
				case SDL_QUIT:
					running = false;
					break;
				case SDL_KEYDOWN:
				{
					switch (event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							running = false;
							break;
						default:
							break;
					}
					break;
				}
			}
		}
	}
}

const char* DEFAULT_SCENE = "data/chess_setup.qdmg";

void setupChess()
{
    for(int i = 0; i < 8; ++i)
    {
        for(int j = 0; j < 8; ++j)
        {
            positions[i][j]= {-175 + i * 50, 0, -175 + j * 50};
        }
    }

    int indexX[] = {0, 7, 1, 6, 2, 5, 3, 4};

    int index = 1;
    for(int i = 0; i < 8; ++i)
    {
        scene.nodes[index]->x = i;
        scene.nodes[index]->y = 1;
        index++;
    }

    for(int i = 0; i < 8; ++i)
    {
        scene.nodes[index]->x = indexX[i];
        scene.nodes[index]->y = 0;
        index++;
    }

    for(int i = 0; i < 8; ++i)
    {
        scene.nodes[index]->x = i;
        scene.nodes[index]->y = 6;
        index++;
    }

    for(int i = 0; i < 8; ++i)
    {
        scene.nodes[index]->x = indexX[i];
        scene.nodes[index]->y = 7;
        index++;
    }
}

int main ( int argc, char** argv )
{
	initRandom(42);
	Color::init_sRGB_cache();
	const char* sceneFile = argc == 2 ? argv[1] : DEFAULT_SCENE;
	if (!scene.parseScene(sceneFile)) {
		printf("Could not parse the scene!\n");
		return -1;
	}

	initGraphics(scene.settings.frameWidth, scene.settings.frameHeight,
		scene.settings.interactive && scene.settings.fullscreen);
	setWindowCaption("Quad Damage: preparing...");

	if (scene.settings.numThreads == 0)
		scene.settings.numThreads = get_processor_count();

	pool.preload_threads(scene.settings.numThreads);

	scene.beginRender();

    setupChess();

    mainloop();

	closeGraphics();
	printf("Exited cleanly\n");
	return 0;
}
