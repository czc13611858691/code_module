/**
 * @file soft_spi.h
 * @author 曹志成 (czc13611858691@gmail.com)
 * @brief 
 * 
 * 参考资料地址:
 * 软件模拟SPI接口程序代码（4种模式）
 * https://blog.csdn.net/zwj695535100/article/details/107303648
 * 
 * spi接口:
 * SCK/CS/MOSI/MISO
 * 
 * 高位在前，低位在后
 * 每个时钟周期，MOSI和MISO的数据在不同的时钟边沿输入
 * 
 * CPOL - 时钟极性
 * 0:空闲,SCK低电平
 * 1:空闲,SCK高电平
 * 
 * CPHA - 时钟相位
 * 0:第一个时钟边沿输入
 * 1:第二个时钟边沿输入
 * 
 * 4种模式
 * MODE0:CPOL=0,CPHA=0
 * MODE1:CPOL=0,CPHA=1
 * MODE2:CPOL=1,CPHA=0
 * MODE3:CPOL=1,CPHA=1
 * 
 * 待解决问题:
 * 1.SPI总线是否容易出现干扰?如何提高总线数据的准确?
 * 
 * @version 0.1
 * @date 2021-11-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include <stdint.h>

/* TODO:根据需要决定spi是否要延时 */
#define SPI_DELAY

typedef enum
{
    SPI_MODE_0,
    SPI_MODE_1,
    SPI_MODE_2,
    SPI_MODE_3,
} SPI_MODE_E;

typedef uint8_t spi_data_t;

typedef struct
{
    void (*mosi_set)(uint8_t);
    uint8_t (*miso_get)(void);
    void (*sck_set)(uint8_t);
    void (*cs_set)(uint8_t);
} spi_ifc_t;

typedef struct
{
    SPI_MODE_E mode;
    spi_ifc_t ifc;
} spi_instance_t;

typedef enum
{
    SPI_RET_OK,
    SPI_RET_ERR,
} SPI_RET_E;

uint8_t SoftSpi_Write(spi_instance_t *spi_instance, spi_data_t *data_trans,uint8_t len);
uint8_t SoftSpi_Read(spi_instance_t *spi_instance, spi_data_t *data_stored,uint8_t len);
uint8_t SoftSpi_ReadWrite(spi_instance_t *spi_instance, spi_data_t *data_trans, spi_data_t *data_stored,uint8_t len);
