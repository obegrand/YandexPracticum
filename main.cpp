#include "request_handler.h"
#include "json_reader.h"
#include "map_renderer.h"
#include "svg.h"

int main() {
	catalogue::TransportCatalogue catalogue;
	JsonReader reader(std::cin, catalogue);
	reader.FillCatalogue();
	RequestHandler(reader, catalogue);
	//render::MapRender render(reader.FillSettings(reader.GetRenderSettings().AsMap()));
	//render.GetMap(catalogue.GetAllBusesSorted()).Render(std::cout);
}