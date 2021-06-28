#include "ScriptComponent.hpp"
#include "Utils.hpp"

namespace PrEngine{

	Scripting::Scripting() : Component(COMP_SCRIPT),script_pos(0),ref_table_pos(0)
	{
		
	}

	Scripting::~Scripting()
	{
		for (auto script : scripts)
		{
			if (script != nullptr)
				delete(script);
		}
	}

	void Scripting::awake()
	{
		for (auto script : scripts)
		{
			if(script != nullptr)
				script->awake();
		}
	}

	void Scripting::start()
	{
		for (auto script : scripts)
		{
			if (script != nullptr)
				script->start();
		}
	}

	void Scripting::update()
	{
		for (auto script : scripts)
		{
			if (script != nullptr)
				script->update();
		}
	}

	void Scripting::end()
	{
		for (auto script : scripts)
		{
			if (script != nullptr)
				script->end();
		}
	}



	Uint_32 Scripting::add_script(Script* script, const char* script_name)
	{

		if (script_pos >= MAX_SCRIPTS_PER_ENTITY)
		{
			LOG(LOGTYPE_ERROR, "Max number of scripts per entity reached");
			return 0;
		}
		else
		{
			scripts[script_pos] = script;
			strcpy(script_names[script_pos], script_name);
			ref_table[ref_table_pos] = script_pos;
			ref_table_rev[script_pos] = ref_table_pos;
			script_pos++;
			ref_table_pos++;
			return ref_table_pos-1;
		}

	}

	void Scripting::remove_script(const char* script_name)
	{
		for (Uint_32 _i = 0; _i < MAX_SCRIPTS_PER_ENTITY; _i++)
		{
			if (strcmp(script_name, script_names[_i]) == 0)
			{
				ref_table[ref_table_rev[_i]] = 0; // reverse lookup _i to get reference_id and map it to null id [0]

				Script* temp = scripts[_i];
				scripts[_i] = scripts[script_pos-1];
				scripts[script_pos] = nullptr;
				script_pos--;

			}
		}
	}

	void Scripting::remove_script(Uint_32 ref_id)
	{
		int script_id = ref_table[ref_id];
		ref_table[ref_id] = 0; 

		if(script_id)
		{
			Script* temp = scripts[script_id];
			scripts[script_id] = scripts[script_pos-1];
			scripts[script_pos] = nullptr;
			script_pos--;
		}
	}

	Script* Scripting::get_script(const char* script_name)
	{
		for (Uint_32 _i = 0; _i < MAX_SCRIPTS_PER_ENTITY; _i++)
		{
			if (strcmp(script_name, script_names[_i]) == 0)
				return scripts[_i];
		}
	}

	Script* Scripting::get_script(Uint_32 ref_id)
	{
		if (ref_table[ref_id] != 0)
		{
			return scripts[ref_table[ref_id]];
		}
		else
			nullptr;
	}

	Uint_32 Scripting::get_script_ref(const char* script_name)
	{
		for (Uint_32 _i = 0; _i < MAX_SCRIPTS_PER_ENTITY; _i++)
		{
			if (strcmp(script_name, script_names[_i]) == 0)
			{
				return ref_table_rev[_i];
			}
		}

		return 0;
	}

	std::string Scripting::to_string()
	{
		return "<Runtime Script>";
	}
}