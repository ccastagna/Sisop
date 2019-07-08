#ifndef INFO_H_INCLUDED
#define INFO_H_INCLUDED

typedef struct t_cabecera {
    int identificador;
    int longitud;
} t_cabecera; 

typedef enum {
    // Tipo de mensajes que enviara el cliente
    id_partido,
    id_patente,
    id_monto,
    id_opcion,
    id_nombre_titular,
    // Tipo de mensajes que enviara el servidor
    id_welcome,
    id_indicate_option,
    id_invalid_option,
    id_ask_patente,
    id_ask_amount,
    id_ask_titular_name,
    id_fee_succesfull,
    id_fee_unsuccesfull,
    id_paid_pending_fee,
    id_not_pending_fee,
    id_common_not_found_message,
    id_menu,
    id_registros_suspender,
    id_buscar_infractor,
    id_monto_total_infractores
} Identificadores;

#endif
