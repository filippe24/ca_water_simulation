#ifndef WATERGRID_H
#define WATERGRID_H
#include <QApplication>
#include <glm/glm.hpp>
#include <iostream>



class waterGrid
{
public:
    waterGrid();
    waterGrid(float room_dim, float cube_dim, int mode)
    {
        room_dim_param = room_dim;
        cube_dimension = cube_dim;
        cubes_per_side = int(room_dim_param*int(room_dim_param/cube_dimension));
        initializeWaterGrid(mode);
    }

    float room_dim_param;
    float cube_dimension;
    int cubes_per_side;


    float dt = 0.1f;
    float h = 1.0f;
    float c = 3.0f;

    //WATER CUBE
    typedef std::pair<uint,uint> positionType;
    struct waterCubeStruct
    {
        uint id;
        positionType position;
        float heigth = 2.0f;
        float velocity = 0.0f;

        void createWaterCube(uint id_in, positionType pos_in)
        {
            id = id_in;
            position = pos_in;
        }

    };
    //WATER STRUCTURE
    std::vector<waterCubeStruct> water;
    void initializeWaterGrid(int mode)
    {
        uint id = 0;
        water.clear();
        for(int col_i = 0; col_i < cubes_per_side; col_i++ )
        {
            for(int row_j = 0; row_j < cubes_per_side; row_j++ )
            {
                waterCubeStruct cube_i_j;
                positionType pos = positionType(col_i, row_j);
                cube_i_j.createWaterCube(id,pos);
                if(mode == 0)
                    cube_i_j.heigth = cube_i_j.heigth + col_i*0.3f;
                else if(mode == 1)
                    cube_i_j.heigth = cube_i_j.heigth + row_j*0.3f;
                else if(mode == 2)
                    cube_i_j.heigth = (cube_i_j.heigth + 2.0f*(1.0f - float(col_i+row_j)/float(2*cubes_per_side)) );



                water.push_back(cube_i_j);
                id++;
            }
        }
    }


    glm::vec3 getCubePosition(uint id)
    {
        glm::vec3 returned_vector = glm::vec3(0.0,0.0,0.0);

        int center = cubes_per_side/2;
        int col = int(water[id].position.first);
        int row = int(water[id].position.second);

        int col_diff = col - center;
        int row_diff = row - center;

        returned_vector.x = cube_dimension/2.0f + col_diff*cube_dimension;
        returned_vector.z = cube_dimension/2.0f + row_diff*cube_dimension;

        returned_vector.y = (cube_dimension/2.0f) * water[id].heigth;

        return returned_vector;
    }

    uint getIdFromPos(int col, int row)
    {
        return uint(col*cubes_per_side + row);
    }
    uint getIdFromPos(positionType cellPos)
    {
        return uint(cellPos.first*uint(cubes_per_side) + cellPos.second);
    }


    void updateWater()
    {
        std::vector<float> new_velocities;
        for(uint w = 0; w < water.size(); w++)
        {

            float old_vel = water[w].velocity;
            float neighH = 0.0f;
            std::vector<uint> neigh = getNeighbours(w);
            for(uint n = 0; n < neigh.size(); n++)
            {
                neighH += water[neigh[n]].heigth;
            }
            float new_vel = old_vel + 0.99f* (neighH/4.0f  - water[w].heigth) ;

            new_vel = 0.99f*(old_vel + dt*c*c * (neighH  - 4.0f*water[w].heigth)/(h*h)) ;

//            std::cout << std::endl;
//            std::cout << " cube " << w  << std::endl;
//            std::cout << " position :" << water[w].position.first << " , " << water[w].position.second << "  and computed id :"<<getIdFromPos(water[w].position) << std::endl;
//            std::cout << " old velocity = " << old_vel << std::endl;
//            std::cout << " old heigth = " << water[w].heigth << std::endl;
//            std::cout << " number of neigh = " << neigh.size() << std::endl;
//            std::cout << "      neigh " << neigh[0] << " , " << neigh[1] << " , " << neigh[2] << ", " << neigh[3] <<std::endl ;
//            std::cout << " new velocity = " << new_vel << std::endl;
//            std::cout << " new heigth = " << water[w].heigth + new_vel << std::endl;



//            water[w].velocity = new_vel;
//            water[w].heigth  += new_vel;

            new_velocities.push_back(new_vel);
        }
        for(uint w = 0; w < water.size(); w++)
        {
            water[w].velocity = new_velocities[w];
            water[w].heigth  += dt*new_velocities[w];
        }



//            if(water[w].heigth < 2.0f)
//            {
//                water[w].heigth = 2.0f;
//                water[w].velocity = 0.0f;
//            }




    }


    std::vector<uint> getNeighbours(uint id)
    {

        std::vector<uint> returned_list;
        uint c = water[id].position.first;
        uint r = water[id].position.second;

        positionType neighPos;
        //1
        if(c > 0)
        {
            neighPos.first = c-1;
            neighPos.second = r;
            returned_list.push_back(getIdFromPos(neighPos));
        }
        else
        {
            neighPos.first = 0;
            neighPos.second = r;
            returned_list.push_back(getIdFromPos(neighPos));
        }
        //2
        if(r > 0)
        {
            neighPos.first = c;
            neighPos.second = r-1;
            returned_list.push_back(getIdFromPos(neighPos));
        }
        else
        {
            neighPos.first = c;
            neighPos.second = 0;
            returned_list.push_back(getIdFromPos(neighPos));
        }
        //3
        if(r < uint(cubes_per_side)-1)
        {
            neighPos.first = c;
            neighPos.second = r+1;
            returned_list.push_back(getIdFromPos(neighPos));
        }
        else
        {
            neighPos.first = c;
            neighPos.second = uint(cubes_per_side)-1;
            returned_list.push_back(getIdFromPos(neighPos));
        }
        //4
        if(c < uint(cubes_per_side)-1)
        {
            neighPos.first = c+1;
            neighPos.second = r;
            returned_list.push_back(getIdFromPos(neighPos));
        }
        else
        {
            neighPos.first = uint(cubes_per_side)-1 ;
            neighPos.second = r;
            returned_list.push_back(getIdFromPos(neighPos));
        }


        return returned_list;


    }




};

#endif // WATERGRID_H
