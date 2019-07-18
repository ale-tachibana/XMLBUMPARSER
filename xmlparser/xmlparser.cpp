#include "stdafx.h"
#include "xml_main.hpp"
#include <string>
#include <iostream>
#include "txtsample.h"

int _tmain(int argc, _TCHAR* argv[])
{

	//sample how to use
	xmlbum::xml_parser xml;	 
	 	
	std::string string_teste;

	int teste;
	
	//can load from file path
	std::string filepath = "../skinned_mesh.dae";	
	std::shared_ptr<xmlbum::xml_item> item = xml.parse_file(filepath);
	
	//can also load from a string
	//std::shared_ptr<xmlbum::xml_item> item = xml.parse_string(xmlsample);
			
	//get subitem, exact string
	std::shared_ptr<xmlbum::xml_item> item2 = item->GetItem("COLLADA")->GetItem("library_geometries")->GetItem("geometry")->GetItem("mesh")->GetItem("source")->GetItem("float_array");
	
	std::cout << item2->value() << "\n";

	//get same object as item2 from root
	std::shared_ptr<xmlbum::xml_item> item3 = item2->root()->GetItem("COLLADA")->GetItem("library_geometries")->GetItem("geometry")->GetItem("mesh")->GetItem("source")->GetItem("float_array");

	std::cout << item3->tag() << "\n";
	

	//search subitems, regex
	std::vector<std::shared_ptr<xmlbum::xml_item>> items4 = item->SearchItems("", "geom");
	for (std::shared_ptr<xmlbum::xml_item> it : items4) {
		std::cout << "found " << it->tag() << "\n";		
	}
	

	//search subitems, exact tag	
	std::vector<std::shared_ptr<xmlbum::xml_item>> items5 = item->SearchItems("float_array","");
	for (std::shared_ptr<xmlbum::xml_item> it : items5)
		std::cout << "found " << it->tag() << "\n";
	

	//get attibutes
	std::vector<std::shared_ptr<xmlbum::xml_item>> items6 = item->SearchItems("technique", "");
	for (std::shared_ptr<xmlbum::xml_item> it : items6) {
		std::cout << "profile: " << it->GetAttribute("profile") << "\n";
		std::cout << "sid: " << it->GetAttribute("sid") << "\n";
	}
	return 0;
}

