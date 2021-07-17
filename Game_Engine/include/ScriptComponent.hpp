#ifndef SCRIPT_COMPONENT_HPP
#define SCRIPT_COMPONENT_HPP

#include <unordered_map>
#include "Component.hpp"
#include "Script.hpp"
#define MAX_SCRIPTS_PER_ENTITY 4

namespace PrEngine
{
	extern std::vector<std::string> script_names;

	struct Scripting : public Component
	{	

		Scripting();
		~Scripting();

		std::string to_string() override;
		void initialize() override;
		void start() override;
		void update() override;
		void end() override;

		Uint_32 add_script(Script* script, Uint_32 name_index);
		void remove_script(const char* script_name);
		void remove_script(Uint_32 ref_id);
		Script* get_script_by_name_index(Uint_32 name_index);
		void get_scripts(Uint_32 name_index, Script* _scripts[MAX_SCRIPTS_PER_ENTITY], Byte_8& count);
		Script* get_script(Uint_32 ref_id);
		const char* get_script_name(Uint_32 ref_id);
		Uint_32 get_script_ref(const char* script_name);
		std::unordered_map<Uint_32, Uint_32> ref_table; // ref_id, script_id
		static Script* (*get_script_instance)(const char* name);


	private:
		Uint_32 script_pos;
		Uint_32 ref_table_pos;
		Uint_32 script_name_indices[MAX_SCRIPTS_PER_ENTITY+1];
		Script* scripts[MAX_SCRIPTS_PER_ENTITY+1];

	};
}
#endif // !SCRIPT_COMPONENT_HPP
