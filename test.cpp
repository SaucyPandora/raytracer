// shared_ptr-examples.cpp
// The following examples assume these declarations:
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct MediaAsset
{
    virtual ~MediaAsset() = default; // make it polymorphic
};

struct Song : public MediaAsset
{
    std::wstring artist;
    std::wstring title;
    Song(const std::wstring& artist_, const std::wstring& title_) :
        artist{ artist_ }, title{ title_ } {}
};

struct Photo : public MediaAsset
{
    std::wstring date;
    std::wstring location;
    std::wstring subject;
    Photo(
        const std::wstring& date_,
        const std::wstring& location_,
        const std::wstring& subject_) :
        date{ date_ }, location{ location_ }, subject{ subject_ } {}
};

using namespace std;

int main()
{
    // The examples go here, in order:
    
   // Use make_shared function when possible.
   auto sp1 = make_shared<Song>(L"The Beatles", L"Im Happy Just to Dance With You");

   // Ok, but slightly less efficient. 
   // Note: Using new expression as constructor argument
   // creates no named variable for other code to access.
   shared_ptr<Song> sp2(new Song(L"Lady Gaga", L"Just Dance"));

   // When initialization must be separate from declaration, e.g. class members, 
   // initialize with nullptr to make your programming intent explicit.
   shared_ptr<Song> sp5(nullptr);
   //Equivalent to: shared_ptr<Song> sp5;
   //...
   sp5 = make_shared<Song>(L"Elton John", L"I'm Still Standing");
   //Initialize with copy constructor. Increments ref count.
   auto sp3(sp2);

   //Initialize via assignment. Increments ref count.
   auto sp4 = sp2;

   //Initialize with nullptr. sp7 is empty.
   shared_ptr<Song> sp7(nullptr);

   // Initialize with another shared_ptr. sp1 and sp2
   // swap pointers as well as ref counts.
   sp1.swap(sp2);
   vector<shared_ptr<Song>> v {
   make_shared<Song>(L"Bob Dylan", L"The Times They Are A Changing"),
   make_shared<Song>(L"Aretha Franklin", L"Bridge Over Troubled Water"),
   make_shared<Song>(L"Thalía", L"Entre El Mar y Una Estrella")
   };

   vector<shared_ptr<Song>> v2;
   remove_copy_if(v.begin(), v.end(), back_inserter(v2), [] (shared_ptr<Song> a) 
   {
      return a->artist.compare(L"Bob Dylan") == 0;
   });

   for (const auto& s : v2)
   {
      wcout << s->artist << L":" << s->title << endl;
   }
    // Example 4
    // Example 6
}