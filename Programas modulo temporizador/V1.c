#include <V1.h>

#include <16f883.h>
#fuses NOWDT, PUT, NOMCLR, BROWNOUT, NOLVP, NOCPD, NOWRT, NODEBUG, NOPROTECT, INTRC_IO, NOFCMEN, NOIESO
#use delay(clock = 20000000) /* Reloj de 20 MHz */

#byte puerto_b = 0x06 /* Dirección del puerto B */

#bit RC0_bit = 0x07.0
#bit RC1_bit = 0x07.1
#bit RC2_bit = 0x07.2

void main() {
    set_tris_b(0x00);
    puerto_b = 0;
    int ignicion = 1;
    int cargaBus = 1;

    while (1) {
        // ENTRADAS
        // RC0 = +15
        // RC1 = Radio
        // RC2 = EV 20-21
        // RC3 = Disponible

        // SALIDAS
        // RB0 = Rutero frontal
        // RB1 = Radio
        // RB2 = NVR
        // RB3 = Switch

        // +15
        if (!RC0_bit) {
            if (ignicion) {
                // Cuando RC0 está en 1, enciende las salidas
                bit_set(puerto_b, 0);  // Enciende RB0
                delay_ms(400);
                bit_set(puerto_b, 2);  // Enciende RB2
                delay_ms(400);  // Escalonado 2 segundos
                bit_set(puerto_b, 3);  // Enciende RB3
                ignicion = 0;
            }
        } else {
            if (!ignicion) {
                // Cuando RC0 cambia a 0, espera 5 minutos
                delay_ms(60000);  // Espera 5 minutos

                // Apaga las salidas
                bit_clear(puerto_b, 0);  // Apaga RB0
                bit_clear(puerto_b, 3);  // Apaga RB3
                delay_ms(12000);
                bit_clear(puerto_b, 2);  // Apaga RB2 apaga 1 minuto después
                ignicion = 1;
            }
        }

        // Activación radio
        if (!RC0_bit && !RC1_bit) {
            bit_set(puerto_b, 1);
        } else {
            bit_clear(puerto_b, 1);
        }

        // EV 20-21
        if (!RC2_bit) {
            if (cargaBus) {  // Escalonado 2 segundos
                bit_set(puerto_b, 2);  // Enciende RB2
                bit_set(puerto_b, 3);  // Enciende RB3
                cargaBus = 0;
            }
        } else {
            if (!cargaBus) {
                delay_ms(24000);
                // Apaga las salidas
                bit_clear(puerto_b, 3);  // Apaga RB3
                bit_clear(puerto_b, 2);  // Apaga RB2
                cargaBus = 1;
            }
        }
    }
}
