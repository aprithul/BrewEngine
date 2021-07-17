#include "ScriptComponent.hpp"
#include "Utils.hpp"

namespace PrEngine{

	Script* (*Scripting::get_script_instance)(const char* name);

	Scripting::Scripting() : Component(COMP_SCRIPT)
	{
		start();
	}

	Scripting::~Scripting()
	{
		for (auto script : scripts)
		{
			if (script != nullptr)
				delete(script);
		}
	}


	void Scripting::initialize()
	{
		script_pos = 1;
		ref_table_pos = 1;
		std::memset(script_name_indices, 0, sizeof(Uint_32)*(MAX_SCRIPTS_PER_ENTITY + 1));
		std::memset(scripts, 0, sizeof(Script*)*(MAX_SCRIPTS_PER_ENTITY + 1));

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



	Uint_32 Scripting::add_script(Script* script, Uint_32 name_index)
	{

		if (script_pos > MAX_SCRIPTS_PER_ENTITY)
		{
			LOG(LOGTYPE_ERROR, "Max number of scripts per entity reached");
			return 0;
		}
		else
		{
			scripts[script_pos] = script;
			script_name_indices[script_pos] = name_index;
			ref_table[ref_table_pos] = script_pos;
			script_pos++;
			ref_table_pos++;
			
			script->start();

			return ref_table_pos-1;
		}

	}

	void Scripting::remove_script(const char* script_name)
	{
		Uint_32 ref = get_script_ref(script_name);
		if (ref)
			remove_script(ref);
	}

	void Scripting::remove_script(Uint_32 ref_id)
	{
		int script_id = ref_table[ref_id];

		if(script_id)
		{
			Script* temp = scripts[script_id];
			scripts[script_id] = scripts[script_pos-1];
			script_name_indices[script_id] = script_name_indices[script_pos - 1];
			script_name_indices[script_pos - 1] = 0;
			scripts[script_pos - 1] = nullptr;

			ref_table[ref_id] = 0;
			for (auto& r : ref_table)
			{
				if (r.second == script_pos - 1)
					ref_table[r.first] = script_id;
			}

			script_pos--;
			delete temp;
		}
	}

	Script* Scripting::get_script_by_name_index(Uint_32 name_index)
	{
		for (Uint_32 _i = 1; _i <= MAX_SCRIPTS_PER_ENTITY; _i++)
		{
			if (name_index == script_name_indices[_i])
				return scripts[_i];
		}
	}

	void Scripting::get_scripts(Uint_32 name_index, Script* _scripts[MAX_SCRIPTS_PER_ENTITY], Byte_8& count)
	{
		for (Uint_32 _i = 1; _i <= MAX_SCRIPTS_PER_ENTITY; _i++)
		{
			if (name_index == script_name_indices[_i])
			{
				_scripts[count] = scripts[_i];
				count++;
			}
		}
	}

	Script* Scripting::get_script(Uint_32 ref_id)
	{
		if (ref_table[ref_id] != 0)
		{
			return scripts[ref_table[ref_id]];
		}
		else
			return nullptr;
	}

	const char* Scripting::get_script_name(Uint_32 ref_id)
	{
		if (ref_table[ref_id] != 0)
		{
			return script_names[script_name_indices[ref_table[ref_id]]].c_str();
		}
		else
			return nullptr;
	}

	Uint_32 Scripting::get_script_ref(const char* script_name)
	{
		for (auto& r : ref_table)
		{
			if (strcmp( script_names[script_name_indices[r.second]].c_str(), script_name) == 0)
			{
				return r.first;
			}
		}

		return 0;
	}

	std::string Scripting::to_string()
	{
		return "<Runtime Script>";
	}
}