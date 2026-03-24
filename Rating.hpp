#pragma once;

class Rating{
    private:
        int userid;
        int movieid;
        double score;
    public:
        Rating(int userid,int movieid,double score);
        
        int getUserid() const;
        int movieId() const;
        double getScore() const;

        void display() const;
};