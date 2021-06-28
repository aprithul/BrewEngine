#ifndef SCRIPT_COMPONENT_HPP
#define SCRIPT_COMPONENT_HPP

#include <unordered_map>
#include "Component.hpp"
#include "Script.hpp"
#define MAX_SCRIPTS_PER_ENTITY 16

namespace PrEngine
{
	struct Scripting : public Component
	{	

		Scripting();
		~Scripting();

		std::string to_string() override;
		void awake() override;
		void start() override;
		void update() override;
		void end() override;

		Uint_32 add_script(Script* script,  const char* script_name);
		void remove_script(const char* script_name);
		void remove_script(Uint_32 ref_id);
		Script* get_script(const char* script_name);
		Script* get_script(Uint_32 ref_id);
		Uint_32 get_script_ref(const char* script_name);


	private:
		Uint_32 script_pos;
		Uint_32 ref_table_pos;
		char script_names[MAX_SCRIPTS_PER_ENTITY][128];
		Script* scripts[MAX_SCRIPTS_PER_ENTITY];
		std::unordered_map<Uint_32, Uint_32> ref_table; // ref_id, script_id
		std::unordered_map<Uint_32, Uint_32> ref_table_rev; // script_id, ref_id
	};
}
#endif // !SCRIPT_COMPONENT_HPP
