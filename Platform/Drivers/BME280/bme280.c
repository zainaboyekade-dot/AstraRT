/*
 * BME280 Sensor Driver
 *
 * Handles:
 * - Device identification
 * - Calibration loading
 * - Temperature measurement
 * - Pressure measurement
 * - Humidity measurement
 *
 * Communication:
 * I2C
 */

#include "bme280.h"
#include "../../HAL/I2C/astra_i2c.h"


#define BME280_CHIP_ID_REG 0xD0
#define BME280_CHIP_ID     0x60

static uint16_t dig_T1;
static int16_t dig_T2;
static int16_t dig_T3;

static uint16_t dig_P1;
static int16_t dig_P2;
static int16_t dig_P3;
static int16_t dig_P4;
static int16_t dig_P5;
static int16_t dig_P6;
static int16_t dig_P7;
static int16_t dig_P8;
static int16_t dig_P9;

static uint8_t dig_H1;
static int16_t dig_H2;
static uint8_t dig_H3;
static int16_t dig_H4;
static int16_t dig_H5;
static int8_t dig_H6;

static int32_t t_fine;


static void BME280_ReadCalibration(void)
{
    uint8_t data[26];


    I2C_ReadRegister(
        BME280_ADDRESS,
        0x88,
        data,
        26
    );


    dig_T1 = (uint16_t)((data[1] << 8) | data[0]);

    dig_T2 = (int16_t)((data[3] << 8) | data[2]);

    dig_T3 = (int16_t)((data[5] << 8) | data[4]);


    dig_P1 = (uint16_t)((data[7] << 8) | data[6]);

    dig_P2 = (int16_t)((data[9] << 8) | data[8]);

    dig_P3 = (int16_t)((data[11] << 8) | data[10]);

    dig_P4 = (int16_t)((data[13] << 8) | data[12]);

    dig_P5 = (int16_t)((data[15] << 8) | data[14]);

    dig_P6 = (int16_t)((data[17] << 8) | data[16]);

    dig_P7 = (int16_t)((data[19] << 8) | data[18]);

    dig_P8 = (int16_t)((data[21] << 8) | data[20]);

    dig_P9 = (int16_t)((data[23] << 8) | data[22]);


    dig_H1 = data[25];


    uint8_t hum_data[7];

    I2C_ReadRegister(
        BME280_ADDRESS,
        0xE1,
        hum_data,
        7
    );


    dig_H2 = (int16_t)((hum_data[1] << 8) | hum_data[0]);

    dig_H3 = hum_data[2];

    dig_H4 = (int16_t)((hum_data[3] << 4) | (hum_data[4] & 0x0F));

    dig_H5 = (int16_t)((hum_data[5] << 4) | (hum_data[4] >> 4));

    dig_H6 = (int8_t)hum_data[6];
}



HAL_StatusTypeDef BME280_Init(void)
{
    uint8_t chipID;

    if(I2C_ReadRegister(
            BME280_ADDRESS,
            BME280_CHIP_ID_REG,
            &chipID,
            1) != HAL_OK)
    {
        return HAL_ERROR;
    }

    if(chipID != BME280_CHIP_ID)
    {
        return HAL_ERROR;
    }

    BME280_ReadCalibration();

    uint8_t ctrl_hum = 0x01;
    if(I2C_WriteRegister(
            BME280_ADDRESS,
            0xF2,
            &ctrl_hum,
            1) != HAL_OK)
    {
        return HAL_ERROR;
    }

    uint8_t ctrl_meas = 0x27;
    I2C_WriteRegister(
        BME280_ADDRESS,
        0xF4,
        &ctrl_meas,
        1
    );

    uint8_t config = 0x00;
    I2C_WriteRegister(
        BME280_ADDRESS,
        0xF5,
        &config,
        1
    );

    return HAL_OK;
}


void BME280_ReadTemperature(float *temperature)
{
    uint8_t data[3];


    I2C_ReadRegister(
        BME280_ADDRESS,
        0xFA,
        data,
        3
    );


    uint32_t adc_T =
        ((uint32_t)data[0] << 12) |
        ((uint32_t)data[1] << 4) |
        (data[2] >> 4);



    int32_t var1;
    int32_t var2;


    var1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) *
            ((int32_t)dig_T2)) >> 11;


    var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) *
              ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) *
              ((int32_t)dig_T3)) >> 14;


    t_fine = var1 + var2;


    *temperature =
        ((t_fine * 5 + 128) >> 8) / 100.0f;
}

void BME280_ReadPressure(float *pressure)
{
    uint8_t data[3];

    I2C_ReadRegister(
        BME280_ADDRESS,
        0xF7,
        data,
        3
    );


    int32_t adc_P =
        ((int32_t)data[0] << 12) |
        ((int32_t)data[1] << 4) |
        (data[2] >> 4);


    int64_t var1;
    int64_t var2;
    int64_t p;


    var1 = ((int64_t)t_fine) - 128000;

    var2 = var1 * var1 * (int64_t)dig_P6;

    var2 = var2 +
        ((var1 * (int64_t)dig_P5) << 17);

    var2 = var2 +
        (((int64_t)dig_P4) << 35);


    var1 = ((var1 * var1 * (int64_t)dig_P3) >> 8)
        + ((var1 * (int64_t)dig_P2) << 12);


    var1 =
        (((((int64_t)1) << 47) + var1))
        * ((int64_t)dig_P1) >> 33;


    if(var1 == 0)
    {
        *pressure = 0;
        return;
    }


    p = 1048576 - adc_P;

    p = (((p << 31) - var2) * 3125) / var1;


    var1 = (((int64_t)dig_P9) * (p >> 13) * (p >> 13)) >> 25;

    var2 = (((int64_t)dig_P8) * p) >> 19;


    p = ((p + var1 + var2) >> 8)
        + (((int64_t)dig_P7) << 4);


    *pressure = (float)p / 256.0f;
}

void BME280_ReadHumidity(float *humidity)
{
    uint8_t data[2];


    I2C_ReadRegister(
        BME280_ADDRESS,
        0xFD,
        data,
        2
    );


    int32_t adc_H =
        ((int32_t)data[0] << 8) |
        data[1];


    int32_t v_x1;


    v_x1 = t_fine - 76800;


    v_x1 =
        (((((adc_H << 14)
        - (((int32_t)dig_H4) << 20)
        - (((int32_t)dig_H5) * v_x1))
        + 16384) >> 15)
        *
        (((((((v_x1 * ((int32_t)dig_H6)) >> 10)
        *
        (((v_x1 * ((int32_t)dig_H3)) >> 11)
        + 32768)) >> 10)
        + 2097152)
        *
        ((int32_t)dig_H2)
        + 8192) >> 14));


    v_x1 =
        v_x1 -
        (((((v_x1 >> 15)
        *
        (v_x1 >> 15)) >> 7)
        *
        ((int32_t)dig_H1)) >> 4);


    if(v_x1 < 0)
    {
        v_x1 = 0;
    }


    if(v_x1 > 419430400)
    {
        v_x1 = 419430400;
    }


    *humidity =
        (float)(v_x1 >> 12) / 1024.0f;
}
