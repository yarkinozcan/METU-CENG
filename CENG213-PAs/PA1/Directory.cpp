#include "Directory.h"

File Directory::getFile(int index) const
{
	if(files.size()>index&&index>=0){
	    return files[index];
	}
	else{
	    File bos;
	    return bos;
	}
}

size_t Directory::directorySize() const
{
	size_t sum=0;
	for(int i=0;i<files.size();i++){
	    sum= sum+files[i].fileSize();
	}
	return sum;
}

bool Directory::isEmpty() const
{
	if(files.size()==0){
	    return true;
	}
	else{
	    return false;
	}
}

size_t Directory::numFiles() const
{
	return files.size();
}

void Directory::newFile(const File &file)
{
	files.push_back(file);
}

void Directory::removeFile(int index)
{
	if(index<files.size()&&index>=0){
	    files.erase(files.begin() + index );
	}
}

void Directory::removeFile(File &file)
{
    if(!isEmpty()){
	    for(int i=0;i<files.size();i++){
	        if(files[i]==file){
	            removeFile(i);
	        }
	    }
    }
}

void Directory::removeBiggestFile()
{
	if(!isEmpty()){
	    size_t sum=0;
	    int index=0;
	    for(int i=0;i<files.size();i++){
	        if(files[i].fileSize()>sum){
	            index=i;
	            sum=files[i].fileSize();
	        }
	    }
	    removeFile(index);
	}
}
