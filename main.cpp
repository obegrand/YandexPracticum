#include "request_handler.h"
#include "json_reader.h"
#include "map_renderer.h"
#include "svg.h"

int main() {
	catalogue::TransportCatalogue catalogue;
	JsonReader reader(std::cin, catalogue);
	reader.FillCatalogue();
	//catalogue.Print();
	render::MapRender map_render(reader.FillSettings(reader.GetRenderSettings().AsMap()));
	//RequestHandler(reader, catalogue);
	map_render.GetMap(catalogue.GetAllBusesSorted()).Render(std::cout);
}