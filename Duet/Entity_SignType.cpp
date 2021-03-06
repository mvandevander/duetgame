#include "Entity_SignType.hpp"
#include "trig_functions_degrees.hpp"
#include "game_elements.hpp"
#include <fstream>

Entity_SignType::Entity_SignType(int x, int y, int w, int h, int rot, int texture_index,int fade_delay_ms)
{
    this->type = "SignType"; //we need the type because it counteracts the ambiguity of polymorphism
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->texture_index = texture_index;
    this->rotation = rot;
    this->fade_delay_ms = fade_delay_ms;
    this->bounds.x = this->x-(this->w/2);
    this->bounds.y = this->y-(this->h/2);
    this->bounds.w = this->w;
    this->bounds.h = this->h;
    fade_state = 0;
    fade_alpha = 0;
}

void Entity_SignType::update(Camera &camera, float parallax_modifier, Player &player)
{
    bool player_is_in_view = false;
    if(fade_state<3)
    {
        player_is_in_view = IsPointInRectangle(player.x,player.y,(int)camera.x,(int)camera.y,camera.width,camera.height);
    }
    if(player_is_in_view){
        if(fade_state==0&&!FadeDelayTimer.IsStarted())
        {
            FadeDelayTimer.Start();
        }
        if(parallax_modifier!=0){
            float corrected_cam_x = ((camera.x)*parallax_modifier);
            float corrected_cam_y = ((camera.y)*parallax_modifier);
            int corrected_cam_w = int(camera.width/parallax_modifier);
            int corrected_cam_h = int(camera.height/parallax_modifier);

            if(fade_state==0&&DoBoxesIntersect(bounds.x,bounds.y,bounds.w,bounds.h,(int)corrected_cam_x,(int)corrected_cam_y,corrected_cam_w,corrected_cam_h))
            {
                fade_state = 1;
            }
            else if(fade_state==1&&fade_alpha==1.0&&!DoBoxesIntersect(bounds.x,bounds.y,bounds.w,bounds.h,(int)corrected_cam_x,(int)corrected_cam_y,corrected_cam_w,corrected_cam_h))
            {
                fade_state = 2;
            }
        }
        else fade_state = 1;
        if(fade_state==1&&FadeDelayTimer.GetTicks()>fade_delay_ms&&fade_alpha<1.0)
        {
            fade_alpha+=.005;
            if(fade_alpha>1.0) fade_alpha = 1.0;
        }
        if(fade_state==2){
            fade_alpha=0;
            fade_state=3;
        }
    }
}



bool Entity_SignType::serialize(fstream& file)
{
    if(!file.is_open()) return false;
    file.write((char*) &type, sizeof(type)); //so that when we read in the file, we can tell which type of entity it is.
    int entity_signtype_version = 2;
    file.write((char*) &entity_signtype_version, sizeof(entity_signtype_version));
    file.write((char*) &x, sizeof(x));
    file.write((char*) &y, sizeof(y));
    file.write((char*) &w, sizeof(w));
    file.write((char*) &h, sizeof(h));
    file.write((char*) &rotation, sizeof(rotation));
    file.write((char*) &fade_delay_ms, sizeof(fade_delay_ms));
    file.write((char*) &texture_index, sizeof(texture_index));
    return true;
}

bool Entity_SignType::unserialize(ifstream& file)
{
    if(!file.is_open()) return false;
    int entity_signtype_version;
    file.read((char*) &entity_signtype_version, sizeof(int));
    if(entity_signtype_version==1){
        file.read((char*) &x, sizeof(x));
        file.read((char*) &y, sizeof(y));
        file.read((char*) &w, sizeof(w));
        file.read((char*) &h, sizeof(h));
        file.read((char*) &rotation, sizeof(rotation));
        file.read((char*) &texture_index, sizeof(texture_index));
        bounds.x = x-(w/2);
        bounds.y = y-(h/2);
        bounds.w = w;
        bounds.h = h;
        return true;
    }
    else if(entity_signtype_version==2){
        file.read((char*) &x, sizeof(x));
        file.read((char*) &y, sizeof(y));
        file.read((char*) &w, sizeof(w));
        file.read((char*) &h, sizeof(h));
        file.read((char*) &rotation, sizeof(rotation));
        file.read((char*) &fade_delay_ms, sizeof(fade_delay_ms));
        file.read((char*) &texture_index, sizeof(texture_index));
        bounds.x = x-(w/2);
        bounds.y = y-(h/2);
        bounds.w = w;
        bounds.h = h;
        return true;
    }
    else return false;
}
