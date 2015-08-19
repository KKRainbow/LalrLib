//
// Created by ssj on 15-8-18.
//

#include "fat32.h"

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

int byteArrayToInt(unsigned char b[]) {

    return (b[0] & 0xFF) 		|
           (b[1] & 0xFF) << 8 	|
           (b[2] & 0xFF) << 16 |
           (b[3] & 0xFF) << 24	;

}
short byteArrayToShort(unsigned char b[]){
    return (short) ((b[0] & 0xFF) 	|
                    (b[1] & 0xFF) << 8) ;
}
void info(BootSector boot_sector){
    cout << boot_sector.sector_size << endl;
    cout << int(boot_sector.sectors_per_cluster) << endl;
    cout << boot_sector.reserved_sectors << endl;
    cout << int(boot_sector.number_of_fats) << endl;
    cout << boot_sector.sectos_per_one_FAT << endl;
    cout << boot_sector.total_sectors << endl;
}

string getFilename( DirectoryEntry Dentries){
    string filename;


    for(int j=0;j<10;j++){
        if(Dentries.filename[j+1] != 0xff && Dentries.filename[j+1] != 0x00  )
            filename.push_back(Dentries.filename[j+1]);
    }
    for(int j=0;j<12;j++){
        if(Dentries.filename[j+14] != 0xff && Dentries.filename[j+14] != 0x00)
            filename.push_back(Dentries.filename[j+14]);
    }
    for(int j=0;j<4;j++){
        if(Dentries.filename[j+28] != 0xff && Dentries.filename[j+28] != 0x00)
            filename.push_back(Dentries.filename[j+28]);
    }


    return filename;
}
void loadEntries(BootSector bs , FILE* fat32img , vector<Fat32Entry> &DEntries, unsigned int CurrentCluster  ){

    DEntries.clear();

    unsigned int FirstDataSector 	=    bs.reserved_sectors + ( bs.number_of_fats * bs.sectos_per_one_FAT) ;
    unsigned int FirstSectorofCluster  =  (CurrentCluster-2) * bs.sectors_per_cluster  + FirstDataSector;
    unsigned int FAT_First_Sector = bs.reserved_sectors;
    DirectoryEntry directory_entry ;
    unsigned int count=0;
    unsigned int NextCluster;
    fseek(fat32img,(FirstSectorofCluster)*bs.sector_size,SEEK_SET);
    string name,temp;
    Fat32Entry fat32entry;
    bool readLongFile = true;
    while(1){

        if(count * 32 >= bs.sector_size * bs.sectors_per_cluster){

            count = 0;

            fseek(fat32img , bs.sector_size*FAT_First_Sector + CurrentCluster*4  ,  SEEK_SET);

            fread(&NextCluster ,  4  ,1,fat32img );

            if(NextCluster > 0 &&  NextCluster < ( bs.total_sectors - bs.reserved_sectors - bs.fat_size_sectors*bs.number_of_fats)/bs.sectors_per_cluster ){

                fseek(fat32img , bs.sector_size*(FirstDataSector + (NextCluster-2)*bs.sectors_per_cluster) ,  SEEK_SET);
                CurrentCluster = NextCluster;

            }
            else{
                break;
            }
        }

        fread(&directory_entry,sizeof(directory_entry),1,fat32img);

        if( directory_entry.filename[0] == 0x00)
            break;
        if(directory_entry.filename[0] == 0xE5){
            count++;
            continue;
        }
        if(directory_entry.filename[0] == 0x2E){
            count++;
            continue;
        }

        if(readLongFile == true){
            temp = getFilename(directory_entry);
            name = temp + name;
            if(directory_entry.filename[0] == 0x41 || directory_entry.filename[0] == 0x01){
                readLongFile = false;

            }
        }
        else{
            fat32entry.filename = name;
            fat32entry.dirEntry = directory_entry;
            DEntries.push_back(fat32entry);
            readLongFile = true;
            name.clear();
            temp.clear();

        }

        count++;
    }
}


int main(int argc,char *argv[]) {

    FILE* fat32img = fopen(argv[1],"rb");
    string	command,dir,entry,file;

    vector<unsigned int> directoryStack;
    BootSector bs;

    fseek(fat32img, 0, SEEK_SET);
    fread(&bs, sizeof(BootSector), 1, fat32img);

    // find the root directory

    unsigned int CurrentCluster = bs.cluster_number_for_the_root_directory;
    vector<Fat32Entry> Dentries;
    loadEntries(bs,fat32img,Dentries,CurrentCluster);
    directoryStack.push_back(CurrentCluster);

    vector<string> directory ;

    unsigned int i;

    while(1){

        if(directory.size() == 0){
            cout << "/> ";
        }
        else if(directory.size() == 1){
            cout << "/" << directory[0] << "> ";
        }
        else{
            cout << ".../" << directory[directory.size()-1] << "> ";
        }

        cin >> command;

        if(command == "quit"){
            return 0;
        }
        else if(command == "info"){
            info(bs);
        }
        else if(command == "ls"){
            for(unsigned int i=0;i<Dentries.size();i=i+1){

                if(Dentries[i].dirEntry.attributes == 0x10){
                    cout << "d " << Dentries[i].filename << endl;
                }
                else if(Dentries[i].dirEntry.attributes == 0x20) {
                    cout << "- " << Dentries[i].filename << endl;
                }
            }
        }
        else if(command == "pwd"){
            if(directory.size() == 0){
                cout << "/" << endl;
                continue;
            }
            for(unsigned int i=0;i<directory.size();i++){
                cout << "/" << directory.at(i) ;
            }
            cout << endl;
        }
        else if(command == "cd"){
            cin >> entry;
            if(entry == ".."){
                if(directory.size() == 0){
                    continue;
                }
                directory.pop_back();
                directoryStack.pop_back();
                loadEntries(bs,fat32img,Dentries,directoryStack[directoryStack.size()-1]);
                continue;
            }
            if(entry == ".")
                continue;

            bool found = false;
            for(i=0;i<Dentries.size();i=i+1){
                if ( entry.compare( Dentries[i].filename) == 0 && Dentries[i].dirEntry.attributes == 0x10 ){
                    unsigned int nextCluster = Dentries[i].dirEntry.high_starting_cluster*256*256 + Dentries[i].dirEntry.low_starting_cluster;
                    directoryStack.push_back(nextCluster);
                    loadEntries(bs,fat32img,Dentries,nextCluster);
                    directory.push_back(entry);
                    found = true;
                    break;
                }
            }
            if( found == false){
                cout << "No such a directory" <<  endl;
            }
        }
        else if(command == "size"){
            cin >> file;
            bool found = false;
            for(i=0;i<Dentries.size();i=i+1){
                if ( file.compare( Dentries[i].filename  ) == 0 ){
                    cout << Dentries[i].dirEntry.file_size << endl;
                    found = true;
                    break;
                }
            }
            if(found == false)
                cout << "No such a file" << endl;
        }
        else if(command == "cat"){
            cin >> file;
            bool found = false;
            for(i=0;i<Dentries.size();i=i+1){

                if ( file.compare( Dentries[i].filename   ) == 0 ){
                    unsigned int FirstDataSector 	=    bs.reserved_sectors + ( bs.number_of_fats * bs.sectos_per_one_FAT) ;
                    unsigned int fileStartCluster = Dentries[i].dirEntry.high_starting_cluster*256*256 + Dentries[i].dirEntry.low_starting_cluster;

                    unsigned int FirstSectorofThisFile  =  (fileStartCluster-2) * bs.sectors_per_cluster  + FirstDataSector;
                    unsigned int FAT_First_Sector = bs.reserved_sectors;
                    unsigned int filesize = Dentries[i].dirEntry.file_size;
                    unsigned int NextCluster;

                    fseek(fat32img,(FirstSectorofThisFile)*bs.sector_size,SEEK_SET);

                    while(1){
                        unsigned char c;
                        for(unsigned i=0;i<filesize;i++){
                            if( i != 0 && i % (bs.sector_size*bs.sectors_per_cluster) == 0){
                                fseek(fat32img , bs.sector_size*FAT_First_Sector + fileStartCluster*4  ,  SEEK_SET);
                                fread(&NextCluster , 4 ,1,fat32img );
                                if(NextCluster > 0 && NextCluster < ( bs.total_sectors - bs.reserved_sectors - bs.fat_size_sectors*bs.number_of_fats)/bs.sectors_per_cluster ){
                                    fseek(fat32img , bs.sector_size*(FirstDataSector + (NextCluster-2)*bs.sectors_per_cluster) ,  SEEK_SET);
                                    fileStartCluster = NextCluster;
                                }
                            }
                            fread(&c,sizeof(c),1,fat32img);
                            printf("%c",c);
                        }
                        break;
                    }
                    found = true;
                    cout << endl;
                    break;
                }
            }
            if(found == false)
                cout << "No such a file" << endl;

        }
        else if(command == "crdt"){
            cin >> entry;
            bool found = false;
            for(i=0;i<Dentries.size();i=i+1){
                if ( entry.compare( Dentries[i].filename ) == 0 ){
                    printf("%04d.%02d.%02d %02d:%02d\n",
                           1980 + (Dentries[i].dirEntry.created_day >> 9), (Dentries[i].dirEntry.created_day >> 5) & 0xF, Dentries[i].dirEntry.created_day & 0x1F,
                           (Dentries[i].dirEntry.created_time_hms >> 11), (Dentries[i].dirEntry.created_time_hms >> 5) & 0x3F);
                    found = true;
                    break;
                }
            }
            if(found == false)
                cout << "No such an entry" << endl;


        }
        else if(command == "moddt"){
            cin >> entry;
            bool found = false;
            for(i=0;i<Dentries.size();i=i+1){
                if ( entry.compare( Dentries[i].filename    ) == 0 ){
                    printf("%04d.%02d.%02d %02d:%02d\n",
                           1980 + (Dentries[i].dirEntry.modify_date >> 9), (Dentries[i].dirEntry.modify_date >> 5) & 0xF, Dentries[i].dirEntry.modify_date & 0x1F,
                           (Dentries[i].dirEntry.modify_time >> 11), (Dentries[i].dirEntry.modify_time >> 5) & 0x3F);
                    found = true;
                    break;
                }
            }
            if(found == false)
                cout << "No such an entry" << endl;
        }
    }
    return 0;
}
