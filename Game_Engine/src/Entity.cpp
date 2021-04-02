#include "Entity.hpp"
#include "Transform3D.hpp"
#include "Sprite.hpp"
#include "Camera3D.hpp"
#include "Animation.hpp"
#include "DirectionalLight.hpp"

namespace PrEngine
{
    
	
    Entity::Entity()
    {
		this->id = -1;
    }
    /*
    std::string Entity::to_string()
    {
    	std::string text = "";
		for (Int_32 type = 0; type < COMP_COUNT_MAX; type++)
		{
			if (has_component[type]) {

				switch (type)
				{
				case COMP_TRANSFORM_3D:
					text += transforms[components[type]].to_string()+"\n";
					break;
				case COMP_CAMERA:
					text += cameras[components[type]].to_string()+"\n";
					break;
				case COMP_ANIMATOR:
					text += animators[components[type]].to_string()+"\n";
					break;
				case COMP_GRAPHICS:
					text += graphics[components[type]].to_string()+"\n";
					break;
				case COMP_LIGHT:
					text += directional_lights[components[type]].to_string()+"\n";
					break;
				default:
					break;
				}

			}
		}
    	text+="~\n";
    	return text;
    }
	*/
}
