//
//  Identifiable.h
//  umeshu++
//
//  Created by Chalupecky Vladimir on 3/28/11.
//  Copyright 2011 Kyushu University. All rights reserved.
//

#ifndef __IDENTIFIABLE_H_INCLUDED__
#define __IDENTIFIABLE_H_INCLUDED__

class Identifiable {
public:
    Identifiable() : id_(0) {}
    Identifiable(int id) : id_(id) {}
    
    int id() const { return id_; }
    void set_id(int id) { id_ = id; }

private:
    int id_;
};

#endif // __IDENTIFIABLE_H_INCLUDED__