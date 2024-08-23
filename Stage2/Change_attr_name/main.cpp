// We need to get all the attribute catalog blocks
#include "Buffer/StaticBuffer.h"
#include "Cache/OpenRelTable.h"
#include "Disk_Class/Disk.h"
#include "FrontendInterface/FrontendInterface.h"
#include <iostream>
#include "string.h"

int main(int argc, char *argv[]) {
  Disk disk_run;

  RecBuffer relCatBuffer(RELCAT_BLOCK);
  RecBuffer attrCatBuffer(ATTRCAT_BLOCK);

  HeadInfo relCatHeader;
  HeadInfo attrCatHeader;

  relCatBuffer.getHeader(&relCatHeader);
  attrCatBuffer.getHeader(&attrCatHeader);

  for (int i{}; i < relCatHeader.numEntries; i++) {

    Attribute relCatRecord[RELCAT_NO_ATTRS]; // will store the record from the relation catalog

    relCatBuffer.getRecord(relCatRecord, i);

    if (strcmp(relCatRecord[RELCAT_REL_NAME_INDEX].sVal, "Students") == 0) {

      for (int j{}; j < attrCatHeader.numEntries; j++) {

        Attribute attrCatRecord[ATTRCAT_NO_ATTRS];
        attrCatBuffer.getRecord(attrCatRecord, j);

        if (strcmp(attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal, "Class") == 0) {

          int attrCount = attrCatHeader.numAttrs;
          int slotCount = attrCatHeader.numSlots;

          unsigned char buffer[BLOCK_SIZE];
          Disk::readBlock(buffer, ATTRCAT_BLOCK);

          int recordSize = attrCount * ATTR_SIZE;
          unsigned char *slotPointer = buffer + HEADER_SIZE + slotCount + (recordSize * j); // pointer to starting of slot with attribute name "Class"
          unsigned char *attrNamePointer = slotPointer + (ATTRCAT_ATTR_NAME_INDEX * ATTR_SIZE);

          char newAttrName[] = "Batch";
          memcpy(attrNamePointer, newAttrName, sizeof(newAttrName));
          Disk::writeBlock(buffer, ATTRCAT_BLOCK);
        }
      }
    }
  }

  return 0;
} 
