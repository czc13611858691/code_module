/**
 * @file soft_spi.c
 * @author 曹志成 (czc13611858691@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "soft_spi.h"
#include <stddef.h>

/**
 * @brief 外部文件调用此模块示例
 * 
 */
#if 0

typedef enum
{
    SPI_INSTANCE_0,
    SPI_INSTANCE_NUM,
} SPI_INSTANCE_E;

spi_instance_t g_spi_handle[SPI_INSTANCE_NUM];

static void _Mosi_Set(uint8_t level)
{
    if (level == 0)
    {
        // TODO:
    }
    else
    {
        // TODO:
    }
}

static uint8_t _Miso_Get(void)
{
    uint8_t ret = 0;
    // TODO:
    // if()
    // {
    //     ret=1;
    // }else{
    //     ret=0;
    // }
    return ret;
}

static void _Spi_CS_Set(uint8_t level)
{
    if (level == 0)
    {
        // TODO:
    }
    else
    {
        // TODO:
    }
}

static void _Spi_SCK_Set(uint8_t level)
{
    if (level == 0)
    {
        // TODO:
    }
    else
    {
        // TODO:
    }
}

void SoftSpi_Init_Example(void)
{
    /* TODO:gpio api 配置 */
    g_spi_handle[SPI_INSTANCE_0].ifc.mosi_set = _Mosi_Set;
    g_spi_handle[SPI_INSTANCE_0].ifc.miso_get = _Miso_Get;
    g_spi_handle[SPI_INSTANCE_0].ifc.cs_set = _Spi_CS_Set;
    g_spi_handle[SPI_INSTANCE_0].ifc.sck_set = _Spi_SCK_Set;

    /* TODO:spi模式配置 */
    // g_spi_handle[SPI_INSTANCE_0].mode=SPI_MODE_0;

    /* 根据不同的模式配置SCK空闲时不同的电平 */
    if ((g_spi_handle[SPI_INSTANCE_0].mode == SPI_MODE_0) || (g_spi_handle[SPI_INSTANCE_0].mode == SPI_MODE_1))
    {
        g_spi_handle[SPI_INSTANCE_0].ifc.sck_set(0);
    }
    else
    {
        g_spi_handle[SPI_INSTANCE_0].ifc.sck_set(1);
    }
}

#endif

uint8_t SoftSpi_Write(spi_instance_t *spi_instance, spi_data_t *data, uint8_t len)
{
    uint8_t ret = 0;
    spi_data_t data_temp = 0;
    if (spi_instance == NULL)
    {
        ret = SPI_RET_ERR;
        return ret;
    }
    ret = SoftSpi_ReadWrite(spi_instance, data, &data_temp, len);
    return ret;
}

uint8_t SoftSpi_Read(spi_instance_t *spi_instance, spi_data_t *data_stored, uint8_t len)
{
    if (spi_instance == NULL)
    {
        return SPI_RET_ERR;
    }
    spi_data_t ret = 0;
    ret = SoftSpi_ReadWrite(spi_instance, 0, data_stored, len);
    return ret;
}

void _Spi_Data_Out(spi_instance_t *spi_instance, spi_data_t *data_trans)
{
    void (*mosi_set)(uint8_t) = spi_instance->ifc.mosi_set;
    spi_data_t data_trans_tmp = (*data_trans);
    if (data_trans_tmp & 0x80)
    {
        mosi_set(1);
    }
    else
    {
        mosi_set(0);
    }
}

void _Spi_Data_In(spi_instance_t *spi_instance, spi_data_t *data_stored)
{
    uint8_t (*miso_get)(void) = spi_instance->ifc.miso_get;
    if (miso_get() == 1)
    {
        (*data_stored)++;
    }
}

uint8_t SoftSpi_ReadWrite(spi_instance_t *spi_instance, spi_data_t *data_trans, spi_data_t *data_stored, uint8_t len)
{
    if (spi_instance == NULL)
    {
        return SPI_RET_ERR;
    }

    void (*mosi_set)(uint8_t) = spi_instance->ifc.mosi_set;
    uint8_t (*miso_get)(void) = spi_instance->ifc.miso_get;
    void (*sck_set)(uint8_t) = spi_instance->ifc.sck_set;
    void (*cs_set)(uint8_t) = spi_instance->ifc.cs_set;

    if (mosi_set == NULL)
    {
        return SPI_RET_ERR;
    }

    if (miso_get == NULL)
    {
        return SPI_RET_ERR;
    }

    if (sck_set == NULL)
    {
        return SPI_RET_ERR;
    }

    if (cs_set == NULL)
    {
        return SPI_RET_ERR;
    }

    uint8_t data_size = sizeof(spi_data_t) * 8;
    uint8_t cpol = 0;
    uint8_t cpha = 0;
    uint8_t first_level = 0;
    uint8_t second_level = 0;

    /**
     * MODE0:CPOL=0,CPHA=0
     * MODE1:CPOL=0,CPHA=1
     * MODE2:CPOL=1,CPHA=0
     * MODE3:CPOL=1,CPHA=1
     */
    if (spi_instance->mode == SPI_MODE_0)
    {
        cpol = 0;
        cpha = 0;
    }
    else if (spi_instance->mode == SPI_MODE_1)
    {
        cpol = 0;
        cpha = 1;
    }
    else if (spi_instance->mode == SPI_MODE_2)
    {
        cpol = 1;
        cpha = 0;
    }
    else if (spi_instance->mode == SPI_MODE_3)
    {
        cpol = 1;
        cpha = 1;
    }
    else
    {
        return SPI_RET_ERR;
    }

    /**
     * CPOL - 时钟极性
     * 0:空闲,SCK低电平-----第一个边沿为上升沿
     * 1:空闲,SCK高电平-----第一个边沿为下降沿
     */
    if (cpol == 0)
    {
        first_level = 1;
        second_level = 0;
    }
    else
    {
        first_level = 0;
        second_level = 1;
    }

    cs_set(0);
    SPI_DELAY;

    for (uint8_t byte_len = 0; byte_len < len; byte_len++)
    {
        spi_data_t data_trans_tmp = *data_trans;
        spi_data_t data_stored_tmp = 0;
        for (uint8_t i = 0; i < data_size; i++)
        {
            if (cpha == 0)
            {
                _Spi_Data_Out(spi_instance, &data_trans_tmp);
                data_trans_tmp <<= 1;
            }
            sck_set(first_level);
            SPI_DELAY;
            if (cpha == 0)
            {
                _Spi_Data_In(spi_instance, &data_stored_tmp);
                if (i != (data_size - 1))
                {
                    data_stored_tmp <<= 1;
                }
            }

            if (cpha == 1)
            {
                _Spi_Data_Out(spi_instance, data_trans);
                data_trans_tmp <<= 1;
            }
            sck_set(second_level);
            SPI_DELAY;
            if (cpha == 1)
            {
                _Spi_Data_In(spi_instance, &data_stored_tmp);
                data_stored_tmp <<= 1;
            }
        }
        data_trans++;
        (*data_stored) = data_stored_tmp;
        data_stored++;
    }

    cs_set(1);
    SPI_DELAY;

    return SPI_RET_OK;
}