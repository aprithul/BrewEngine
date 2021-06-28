#include <string>
#include "Scripts/FooScript.hpp"
#include "Scripts/TestScript.hpp"
PrEngine::Script* get_script_of_type(const char* type_name)
{
	if(strcmp(type_name,"Foo")==0){
		return new Foo();
	}
	if(strcmp(type_name,"Test")==0){
		return new Test();
	}
}