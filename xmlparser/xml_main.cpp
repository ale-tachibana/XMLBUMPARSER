/*
MIT License

Copyright (c) 2017 Alessandro K. Tachibana

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "stdafx.h"
#include "xml_main.hpp"

namespace xmlbum
{

	XMLBUM_INLINE std::vector<std::shared_ptr<xml_item>> xml_item::items(){
		return xml_subitems;
	}

	std::shared_ptr<xml_item> xml_item::Item(const std::string tag){
		if (xml_tag_index->count(tag))
		{			
			return xml_subitems[xml_tag_index->equal_range(tag).first->second];
		}
		else
		{
			return nullptr;
		}		
	}
	
	std::vector<std::shared_ptr<xml_item>> xml_item::GetItems(const std::string tag){				
		std::vector<std::shared_ptr<xml_item>> tmpvector;

		for (std::multimap<std::string, int>::iterator it = xml_tag_index->equal_range(tag).first; it != xml_tag_index->equal_range(tag).second; ++it)
		{
			tmpvector.push_back(xml_subitems[it->second]);
		}

		return tmpvector;
	}
	
	std::shared_ptr<xml_item> xml_item::GetItem(const std::string tag){
		return Item(tag);
	}
	
	std::shared_ptr<xml_item> xml_item::GetItem(const std::string tag, const int index){
		return (GetItems(tag))[index];
	}
	
	/*
	std::shared_ptr<xml_item>xml_item::SearchItems(std::string tag){
		
	}
	*/
	std::vector<std::shared_ptr<xml_item>> xml_item::SearchItems(const std::string exacttag, const std::string regextag){
		
		int horizonalcounter = 0;
		int verticalcounter = 0;
				
		std::map<xml_item*, bool> checkedobjs;					
		std::shared_ptr<xml_item> current_item;		
		std::regex searchregex(regextag);
		
		current_item = shared_from_this();

		xml_searchitems.clear();

		bool endfile = false;
		do
		{

			if (checkedobjs.count(current_item.get()) > 0) {
				//get next
				bool nextitem = false;
				do{
					current_item = current_item->parent();
					--verticalcounter;

					for (std::shared_ptr<xml_item> item : current_item->xml_subitems) {
						if (checkedobjs.count(item.get()) == 0) {
							++verticalcounter;
							current_item = item;
							nextitem = true;
							break;
						}
					}
					if ((!nextitem) && (current_item.get() == this))
						endfile = true;
					
				} while ((!nextitem) && (current_item.get() != this));
			}
			
			if(checkedobjs.count(current_item.get()) == 0) {
				checkedobjs[current_item.get()] = true;
				//if there is a match
				std::smatch m;
				if ((exacttag != "" && current_item->xml_tag.compare(exacttag) == 0)
					|| (regextag != "" && std::regex_search(current_item->xml_tag, m, searchregex))) {
										
					xml_searchitems.push_back(current_item);
				}
			}
						
			if (current_item->xml_subitems.size() > 0) {
				current_item = current_item->xml_subitems[0];
				++verticalcounter;
			}						
		} while (!endfile);

		return xml_searchitems;
	}

	XMLBUM_INLINE
	std::vector<std::shared_ptr<xml_item>> xml_item::searchitems()
	{
		return xml_searchitems;
	}
		
	
	std::string xml_item::GetAttribute(const std::string attrname){
		try
		{
			return xml_attributes.at(attrname);
		}
		catch (...)
		{
			return "";
		}
	}

	void xml_item::SplitString(std::string input, std::vector<std::string>& out_string, char delimiter)
	{
		std::stringstream sstream(input);
		std::string temp;

		while (std::getline(sstream, temp, delimiter)) {
			out_string.push_back(temp);
		}
	}
	
	template <typename T> T xml_item::GetAttributeT(const std::string attrname){
		std::string tmpstr;
		try {
			tmpstr = xml_attributes.at(attrname);
		}
		catch (...)
		{
			tmpstr = "";
		}

		T value;
		std::stringstream convert(tmpstr);
		
		convert >> value;

		return value;
	}
		
	std::string xml_item::SearchValueOfItem(const std::string tag){		
		if (xml_tag_index->count(tag)) 	{
			return xml_subitems[xml_tag_index->equal_range(tag).first->second]->value();
		}
		else
		{
			return "";
		}		
	}
	
	
	void xml_item::SetAttributes(const std::map<std::string, std::string> attributes){
		this->xml_attributes = attributes;
	}
	
	std::shared_ptr<xml_item> xml_item::Add(const std::string tag, const std::string value){

		std::shared_ptr<xml_item> item = std::make_shared<xml_item>();

		item->xml_tag = tag;
		item->xml_value = value;
		item->xml_parent = shared_from_this();
		if (!this->root()) {
			item->xml_root = shared_from_this();
		}
		else {
			item->xml_root = this->root();
		}
		
		xml_subitems.push_back(item);
		xml_tag_index->insert(std::make_pair(tag, (xml_subitems.size() - 1)));
		
		return item;
	}
		
	XMLBUM_INLINE std::shared_ptr<xml_item> xml_item::parent()
	{
		return this->xml_parent;
	}

	XMLBUM_INLINE std::shared_ptr<xml_item> xml_item::root()
	{
		return this->xml_root;
	}

	XMLBUM_INLINE std::string xml_item::value()
	{
		return this->xml_value;
	}

	XMLBUM_INLINE void xml_item::value(std::string value)
	{
		this->xml_value = value;
	}

	XMLBUM_INLINE std::string xml_item::tag()
	{
		return this->xml_tag;
	}
	
	xml_item::xml_item(){
		//xml_subitems = new std::vector<xml_item*>();
		//xml_tag_index = new std::multimap < std::string, int >();
		//xml_tag_index.reset(new std::multimap < std::string, int >());
		xml_tag_index = std::make_unique<std::multimap<std::string, int>>();
	}
	xml_item::xml_item(const std::string tag
		              , const std::string value
		              , std::shared_ptr<xml_item> xml_parent
		              , std::shared_ptr<xml_item> xml_root) //:tag(tag), value(value)
	{
		this->xml_tag = tag;
		this->xml_value = value;
		this->xml_parent = xml_parent;
		this->xml_root = xml_root;

		xml_tag_index = std::make_unique<std::multimap<std::string, int>>();
	}
	xml_item::xml_item(const xml_item& other) {
			
		xml_tag_index = std::make_unique<std::multimap<std::string, int>>();
	}

	/*
	xml_item::~xml_item(){
		
		//xml_subitems->clear();
		if (xml_subitems != nullptr){
			if (xml_subitems->max_size() > 0) {
				for (xml_item * item : *xml_subitems) {
					if (item != nullptr) {
						delete(item);
					}
				}
			}			
		}
		
		delete xml_subitems;
		//delete xml_tag_index;
	}
	*/
			
		

	//////////////////////////////////////////////////////////////
	//
	//xml_parser
	//////////////////////////////////////////////////////////////
	xml_parser::xml_parser() {

	}

	std::string xml_parser::trim(std::string str_in){
		size_t p = str_in.find_first_not_of(" \t");
		str_in.erase(0, p);
		
		p = str_in.find_last_not_of(" \t");
		if (std::string::npos != p)
			str_in.erase(p + 1);

		return str_in;
	}
	

	std::shared_ptr<xml_item> xml_parser::parse_istream(std::istream * objFile)
	{
		std::shared_ptr<xml_item> items = std::make_shared<xml_item>();
		std::shared_ptr<xml_item> tmp_item = items;

		std::string line;

		std::string buffer = "";
		std::string bufferPrev = "";
		std::string bufferElement = "";

		std::string tag = "";
		std::string value = "";
		std::string closetag = "";

		std::vector<std::string> stringvector;
		std::map<std::string, std::string> attr_map;

		char lastchar = ' ';
		bool angle_bracket_open = false, angle_bracket_close = false, forward_slash = false;
		bool open = false, close = false, same = false;
		bool up = false, down = false;
		bool skip = false;
		
		while (getline(*objFile, line))
		{
			for (char & c : line)
			{
				if (c == '>' && buffer != "" && bufferElement.substr(0, 2) != "</")
				{
					if (lastchar == '/') {
						parse_attrib(bufferPrev, tag, attr_map);
					}
					else
					{
						parse_attrib(buffer, tag, attr_map);
					}
					buffer = "";
				}
				if (c == '<' && buffer != "") {
					if (trim(buffer) != "") {
						value = buffer;
					}
					buffer = "";
				}
				if (c == '>' && forward_slash && buffer != "")
				{
					closetag = buffer;
					buffer = "";
				}

				//-----------------------------------------------------------
				if ((c == '>' && lastchar == '/'  &&  buffer == "" && tag != "") || (tag == "?xml"))
				{
					same = true;
				}
				else if (c == '>'  && closetag != "" && bufferElement.substr(0, 2) == "</")
				{
					down = true;
				}
				if (tag != "" && !down && c == '>' && !same)
				{
					up = true;
				}

				if (c != '<' && c != '>') {
					bufferPrev = buffer;
					buffer = buffer + c;
				}
				if (buffer == "") {
					bufferElement = "";
				}

				if (c != ' ') {
					bufferElement = bufferElement + c;
				}


				if (value != "")
				{
					tmp_item->value(value);
					value = "";
				}

				if (closetag != "")
				{
					if (closetag != tmp_item->tag())
					{

					}
					else
					{
						closetag = "";
						down = true;
					}
				}




				///////////////////////////
				if (skip)
				{

					skip = false;
				}

				if (up || down)
				{
					angle_bracket_open = false;
					angle_bracket_close = false;
					forward_slash = false;
				}

				if (up)
				{
					tmp_item = tmp_item->Add(tag, value);
					tmp_item->SetAttributes(attr_map);
					//tmp_item->root = items;

					value = "";
					tag = "";
					attr_map.clear();
					up = false;
				}
				if (down)
				{
					if (tmp_item->parent() != NULL)
					{
						tmp_item = tmp_item->parent();
					}
					tag = "";
					value = "";
					attr_map.clear();
					down = false;
				}
				if (same)
				{
					tmp_item = tmp_item->Add(tag, value);
					tmp_item->SetAttributes(attr_map);
					//tmp_item->root = items;

					if (tmp_item->parent() != NULL)
					{
						tmp_item = tmp_item->parent();
					}

					value = "";
					tag = "";
					same = false;
				}
				/////////////////////////////



				if (c != ' ') {
					lastchar = c;
				}

				if (c == '<') {
					angle_bracket_open = true;
				}
				if (c == '/') {
					forward_slash = true;
				}
				if (c == '>') {
					angle_bracket_close = true;
					bufferElement = "";
				}
			}
		}
		
		return items;
	}
	
	void xml_parser::parse_attrib(std::string strattr, std::string &tag, std::map<std::string, std::string> &xml_attributes){

		std::string buffer = "";
		std::string attribname = "";
		std::string attribvalue = "";

		bool openquote = false;

		
		for (char & c : strattr)
		{

			if (buffer != "" && c == ' ' && tag == "")
			{
				tag = buffer;
				buffer = "";
			}
			if (c == '=')
			{
				attribname = trim(buffer);
				buffer = "";
			}
			else if (c == '\"' && openquote)
			{
				attribvalue = buffer;
			}

			buffer = buffer + c;
			
			if (c == '\"')
			{
				if (!openquote)
				{
					openquote = true;
					buffer = "";
				}
				else
				{
					openquote = false;					
					buffer = "";					
				}
			}


			if (attribname != "" && attribvalue != "")
			{
				xml_attributes[attribname] = attribvalue;

				attribname = "";
				attribvalue = "";
			}

		}

		if (tag == "")
		{
			tag = strattr;
		}

	}

	//////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////
	
}