#include "File.h"

Block File::getBlock(int index) const
{
	if(index<blocks.getSize()&&index>=0){
	    return blocks.getNodeAtIndex(index)->data;
	}
	else{
        Block bos;
        return bos;
	}
}

size_t File::fileSize() const
{
	size_t sum=0;
	Node<Block> *current=blocks.getFirstNode();
	while(current){
	    sum+=current->data.getSize();
	    current=current->next;
	}
	return sum;
}

int File::numBlocks() const
{
	return blocks.getSize();
}

bool File::isEmpty() const
{
	Node<Block> *current=blocks.getFirstNode();
	if(current){
	    return false;
	}
	else{
	    return true;
	}
}

bool File::operator==(const File &rhs) const
{
	if(numBlocks()==rhs.numBlocks()){
	    for(int i=0;i<numBlocks();i++){
	        if(!(getBlock(i)==rhs.getBlock(i))){
	            return false;
	        }
	    }
	    return true;
	}
	else{
	    return false;
	}
}

void File::newBlock(const Block &block)
{
	blocks.append(block);
}

void File::removeBlock(int index)
{
	if(index>=0&&index<numBlocks()){
	    blocks.removeNodeAtIndex(index);
	}
}

void File::mergeBlocks(int sourceIndex, int destIndex)
{
	blocks.mergeNodes(sourceIndex,destIndex);
}

void File::printContents() const{
	if(isEmpty()){
		std::cout << "File is empty" << std::endl;
	}
	else{
		for(int i = 0; i < blocks.getSize(); i++){
			Node<Block> *block = blocks.getNodeAtIndex(i);
			std::cout << block->data.getContent();
		}
		std::cout << std::endl << fileSize() << std::endl;
	}
}