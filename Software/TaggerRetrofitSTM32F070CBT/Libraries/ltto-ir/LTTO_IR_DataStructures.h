#ifndef __LTTO_IR_DATA_STRUCTURES_H__
#define __LTTO_IR_DATA_STRUCTURES_H__

#include <stdint.h>
#include <stdbool.h>

//Storage types for all the nitty gritty details
typedef enum {
    LTTO_IR_RAW_SIGNATURE_HEADER_UNKNOWN = 0,
    LTTO_IR_RAW_SIGNATURE_HEADER_NORMAL,
    LTTO_IR_RAW_SIGNATURE_HEADER_BEACON,
} LTTO_IR_RAW_SIGNATURE_HEADER_TYPES_t;

typedef enum {
    LTTO_IR_RAW_SIGNATURE_BIT_COUNT_UNKNOWN = 0,
    LTTO_IR_RAW_SIGNATURE_BIT_COUNT_5,
    LTTO_IR_RAW_SIGNATURE_BIT_COUNT_7,
    LTTO_IR_RAW_SIGNATURE_BIT_COUNT_8,
    LTTO_IR_RAW_SIGNATURE_BIT_COUNT_9,
} LTTO_IR_RAW_SIGNATURE_BIT_COUNTS_t;

typedef enum {
    LTTO_IR_RAW_SIGNATURE_SFP_UNKNOWN = 0,
    LTTO_IR_RAW_SIGNATURE_SFP_SHORT,    //Like beacons, 25ms
    LTTO_IR_RAW_SIGNATURE_SFP_MEDIUM,   //Like tags, 56ms
    LTTO_IR_RAW_SIGNATURE_SFP_LONG,     //For after finishing a multibyte message, 80ms
} LTTO_IR_RAW_SIGNATURE_SFPS_t;

typedef struct {
    LTTO_IR_RAW_SIGNATURE_HEADER_TYPES_t headerType;
    LTTO_IR_RAW_SIGNATURE_BIT_COUNTS_t bitCount;
    uint16_t data;
    LTTO_IR_RAW_SIGNATURE_SFPS_t SFP;
    bool valid;
} LTTO_IR_RAW_SIGNATURE_t;

//Simplify out some of the details
typedef enum {
    LTTO_IR_SIGNATURE_TYPE_UNKNOWN = 0,
    LTTO_IR_SIGNATURE_TYPE_LTTO_BEACON,
    LTTO_IR_SIGNATURE_TYPE_LTAR_BEACON,
    LTTO_IR_SIGNATURE_TYPE_TAG,
    LTTO_IR_SIGNATURE_TYPE_MULTIBYTE_PTYPE,
    LTTO_IR_SIGNATURE_TYPE_MULTIBYTE_DATA,
    LTTO_IR_SIGNATURE_TYPE_MULTIBYTE_CSUM,
} LTTO_IR_SIGNATURE_TYPES_t;

typedef struct {
    LTTO_IR_SIGNATURE_TYPES_t signatureType;
    uint16_t data;
} LTTO_IR_SIGNATURE_t;

//And a storage type for a multibyte message
typedef struct {
    uint8_t dataBytes[22];  //22 byte packets are longest accepted by all taggers.
    uint8_t dataByteCount;
    uint8_t runningChecksum;
    bool valid;
} LTTO_IR_MULTIBYTE_t;

#endif