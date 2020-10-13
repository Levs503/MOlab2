//
// Created by Лев on 21.09.2020.
//

#include "Answer.h"



std::ostream& operator<<(std::ostream& out, Answer& ans){
    out<<"extremum is "<<ans.extremum<<std::endl;
    for(size_t g =0; g< ans.values.size(); g++){
        out<<"X"<<g+1<<" = "<<ans.values[g]<<" ";
    }
    out<<std::endl;
    return out;


}
