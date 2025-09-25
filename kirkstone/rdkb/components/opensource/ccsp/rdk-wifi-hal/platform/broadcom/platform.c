#include <stddef.h>
#include "wifi_hal.h"
#include "wifi_hal_priv.h"
#if defined(WLDM_21_2)
#include "wlcsm_lib_api.h"
#else
#include "nvram_api.h"
#endif // defined(WLDM_21_2)
#include "wlcsm_lib_wl.h"
#if defined (ENABLED_EDPD) && defined(_SR213_PRODUCT_REQ_)
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#endif // defined (ENABLED_EDPD) && defined(_SR213_PRODUCT_REQ_)

#if defined(TCXB7_PORT) || defined(TCXB8_PORT) || defined(XB10_PORT)
#include <rdk_nl80211_hal.h>
#endif // TCXB7_PORT || TCXB8_PORT || XB10_PORT

#define BUFFER_LENGTH_WIFIDB 256
#define BUFLEN_128  128
#define BUFLEN_256 256

typedef struct wl_runtime_params {
    char *param_name;
    char *param_val;
}wl_runtime_params_t;

static wl_runtime_params_t g_wl_runtime_params[] = {
    {"he color_collision", "0x7"},
    {"nmode_protection_override", "0"},
    {"protection_control", "0"},
    {"gmode_protection_control", "0"}
};

static void set_wl_runtime_configs (const wifi_vap_info_map_t *vap_map);
int sta_disassociated(int ap_index, char *mac, int reason);
int sta_deauthenticated(int ap_index, char *mac, int reason);
int sta_associated(int ap_index, wifi_associated_dev_t *associated_dev);
#if defined (ENABLED_EDPD) && defined(_SR213_PRODUCT_REQ_)
static int check_edpdctl_enabled();
static int check_dpd_feature_enabled();
static int enable_echo_feature_and_power_control_configs(void);
int platform_set_ecomode_for_radio(const int wl_idx, const bool eco_pwr_down);
int platform_set_gpio_config_for_ecomode(const int wl_idx, const bool eco_pwr_down);
#endif // defined (ENABLED_EDPD) && defined(_SR213_PRODUCT_REQ_)

static char const *bss_nvifname[] = {
    "wl0",      "wl1",
    "wl0.1",    "wl1.1",
    "wl0.2",    "wl1.2",
    "wl0.3",    "wl1.3",
    "wl0.4",    "wl1.4",
    "wl0.5",    "wl1.5",
    "wl0.6",    "wl1.6",
    "wl0.7",    "wl1.7",
    "wl2",      "wl2.1",
    "wl2.2",    "wl2.3",
    "wl2.4",    "wl2.5",
    "wl2.6",    "wl2.7",
};  /* Indexed by apIndex */

static int get_ccspwifiagent_interface_name_from_vap_index(unsigned int vap_index, char *interface_name)
{
    // OneWifi interafce mapping with vap_index
    unsigned char l_index = 0;
    unsigned char total_num_of_vaps = 0;
    char *l_interface_name = NULL;
    wifi_radio_info_t *radio;

    for (l_index = 0; l_index < g_wifi_hal.num_radios; l_index++) {
        radio = get_radio_by_rdk_index(l_index);
        total_num_of_vaps += radio->capab.maxNumberVAPs;
    }

    if ((vap_index >= total_num_of_vaps) || (interface_name == NULL)) {
        wifi_hal_error_print("%s:%d: Wrong vap_index:%d \n",__func__, __LINE__, vap_index);
        return RETURN_ERR;
    }

    l_interface_name = bss_nvifname[vap_index];
    if(l_interface_name != NULL) {
        strncpy(interface_name, l_interface_name, (strlen(l_interface_name) + 1));
        wifi_hal_dbg_print("%s:%d: VAP index %d: interface name %s\n", __func__, __LINE__, vap_index, interface_name);
    } else {
        wifi_hal_error_print("%s:%d: Interface name not found:%d \n",__func__, __LINE__, vap_index);
        return RETURN_ERR;
    }
    return RETURN_OK;
}

static void set_wl_runtime_configs (const wifi_vap_info_map_t *vap_map)
{
    if (NULL == vap_map) {
        wifi_hal_error_print("%s:%d: Invalid parameter error!!\n",__func__, __LINE__);
        return;
    }

    int wl_elems_index = 0;
    int radio_index = 0;
    int vap_index = 0;
    char sys_cmd[128] = {0};
    char interface_name[8] = {0};
    wifi_vap_info_t *vap = NULL;
    int no_of_elems = sizeof(g_wl_runtime_params) / sizeof(wl_runtime_params_t);

    /* Traverse through each radios and its vaps, and set configurations for private interfaces. */
    for(radio_index = 0; radio_index < g_wifi_hal.num_radios; radio_index++) {
        if (vap_map != NULL) {
            for(vap_index = 0; vap_index < vap_map->num_vaps; vap_index++) {
                vap = &vap_map->vap_array[vap_index];
                if (is_wifi_hal_vap_private(vap->vap_index)) {
                    memset (interface_name, 0 ,sizeof(interface_name));
                    get_interface_name_from_vap_index(vap->vap_index, interface_name);
                    for (wl_elems_index = 0; wl_elems_index < no_of_elems; wl_elems_index++) {
                        snprintf(sys_cmd, sizeof(sys_cmd), "wl -i %s %s %s", interface_name, g_wl_runtime_params[wl_elems_index].param_name, g_wl_runtime_params[wl_elems_index].param_val);
                        wifi_hal_dbg_print("%s:%d: wl sys_cmd = %s \n", __func__, __LINE__,sys_cmd);
                        system(sys_cmd);
                    }
                }
            }
            vap_map++;
        }
    }
}


int sta_disassociated(int ap_index, char *mac, int reason)
{
    return 0;
}

int sta_deauthenticated(int ap_index, char *mac, int reason)
{
    return 0;
}

int sta_associated(int ap_index, wifi_associated_dev_t *associated_dev)
{
    return 0;
}

void prepare_param_name(char *dest, char *interface_name, char *prefix)
{
    memset(dest, 0, strlen(dest));

    strncpy(dest, interface_name, strlen(interface_name));
    strcat(dest, prefix);
}

void set_decimal_nvram_param(char *param_name, unsigned int value)
{
    char temp_buff[8];
    memset(temp_buff, 0 ,sizeof(temp_buff));

    snprintf(temp_buff, sizeof(temp_buff), "%d", value);
#if defined(WLDM_21_2)
    wlcsm_nvram_set(param_name, temp_buff);
#else
    nvram_set(param_name, temp_buff);
#endif // defined(WLDM_21_2)
}

void set_string_nvram_param(char *param_name, char *value)
{
#if defined(WLDM_21_2)
    wlcsm_nvram_set(param_name, value);
#else
    nvram_set(param_name, value);
#endif // defined(WLDM_21_2)
}

int platform_pre_init()
{
    wifi_hal_dbg_print("%s:%d \r\n", __func__, __LINE__);

    system("sysevent set multinet-up 13");
    system("sysevent set multinet-up 14");
    wifi_hal_info_print("sysevent sent to start mesh bridges\r\n");

//    nvram_set("wl0_bw_cap", "3");
    /* registering the dummy callbacks to receive the events in plume */
    wifi_newApAssociatedDevice_callback_register(sta_associated);
    wifi_apDeAuthEvent_callback_register(sta_deauthenticated);
    wifi_apDisassociatedDevice_callback_register(sta_disassociated);
#if 0
    system("wl -i wl0.1 nmode_protection_override 0");
    system("wl -i wl1.1 nmode_protection_override 0");
    system("wl -i wl0.1 protection_control 0");
    system("wl -i wl1.1 protection_control 0");
    system("wl -i wl0.1 gmode_protection_control 0");
    system("wl -i wl1.1 gmode_protection_control 0");
    wifi_hal_dbg_print("%s:%d: wifi param set success\r\n", __func__, __LINE__);
#endif
    return 0;
}

int platform_set_radio_pre_init(wifi_radio_index_t index, wifi_radio_operationParam_t *operationParam)
{
    if ((index < 0) || (operationParam == NULL)) {
        wifi_hal_dbg_print("%s:%d Invalid Argument \n", __FUNCTION__, __LINE__);
        return -1;
    }

    char temp_buff[BUF_SIZE];
    char param_name[NVRAM_NAME_SIZE];
    char cmd[BUFLEN_128];
    wifi_radio_info_t *radio;
    radio = get_radio_by_rdk_index(index);
    if (radio == NULL) {
        wifi_hal_dbg_print("%s:%d:Could not find radio index:%d\n", __func__, __LINE__, index);
        return RETURN_ERR;
    }

#if defined (ENABLED_EDPD) && defined(_SR213_PRODUCT_REQ_)
    int ret = 0;
    if (operationParam->EcoPowerDown) {
        /* Enable eco mode feature and power control configurations. */
        ret = enable_echo_feature_and_power_control_configs();
        if (ret != RETURN_OK) {
            wifi_hal_error_print("%s:%d: Failed to enable EDPD ECO Mode feature\n", __func__, __LINE__);
        }

        //Enable ECO mode for radio
        ret = platform_set_ecomode_for_radio(index, true);
        if (ret != RETURN_OK) {
           wifi_hal_dbg_print("%s:%d: Failed to enable ECO mode for radio index:%d\n", __func__, __LINE__, index);
        }

        //Disconnect the GPIO
        ret = platform_set_gpio_config_for_ecomode(index, true);
        if (ret != RETURN_OK) {
            wifi_hal_dbg_print("%s:%d: Failed to disconnect gpio for radio index:%d\n", __func__, __LINE__, index);
        }
    } else {
        /* Enable eco mode feature and power control configurations. */
        ret = enable_echo_feature_and_power_control_configs();
        if (ret != RETURN_OK) {
            wifi_hal_error_print("%s:%d: Failed to enable EDPD ECO Mode feature\n", __func__, __LINE__);
        }

        //Connect the GPIO
        ret = platform_set_gpio_config_for_ecomode(index, false);
        if (ret != RETURN_OK) {
            wifi_hal_dbg_print("%s:%d: Failed to connect gpio for radio index:%d\n", __func__, __LINE__, index);
        }

        //Disable ECO mode for radio
        ret = platform_set_ecomode_for_radio(index, false);
        if (ret != RETURN_OK) {
            wifi_hal_dbg_print("%s:%d: Failed to disable ECO mode for radio index:%d\n", __func__, __LINE__, index);
        }
    }
#endif // defined (ENABLED_EDPD) && defined(_SR213_PRODUCT_REQ_)

    if (radio->radio_presence == false) {
        wifi_hal_dbg_print("%s:%d Skip this radio %d. This is in sleeping mode\n", __FUNCTION__, __LINE__, index);
        return 0;
    }

    if (radio->oper_param.countryCode != operationParam->countryCode) {
        memset(temp_buff, 0 ,sizeof(temp_buff));
        get_coutry_str_from_code(operationParam->countryCode, temp_buff);
        if (wifi_setRadioCountryCode(index, temp_buff) != RETURN_OK) {
            wifi_hal_dbg_print("%s:%d Failure in setting country code as %s in radio index %d\n", __FUNCTION__, __LINE__, temp_buff, index);
            return -1;
        }

        if (wifi_applyRadioSettings(index) != RETURN_OK) {
            wifi_hal_dbg_print("%s:%d Failure in applying Radio settings in radio index %d\n", __FUNCTION__, __LINE__, index);
            return -1;
        }

        //Updating nvram param
        memset(param_name, 0 ,sizeof(param_name));
        sprintf(param_name, "wl%d_country_code", index);
        set_string_nvram_param(param_name, temp_buff);
    }

    if (radio->oper_param.autoChannelEnabled != operationParam->autoChannelEnabled) {
        memset(cmd, 0 ,sizeof(cmd));
        if (operationParam->autoChannelEnabled == true) {
            /* Set acsd2 autochannel select mode */
            wifi_hal_dbg_print("%s():%d Enabling autoChannel in radio index %d\n", __FUNCTION__, __LINE__, index);
            sprintf(cmd, "acs_cli2 -i wl%d mode 2 &", index);
            system(cmd);

            /* Run acsd2 autochannel */
            memset(cmd, 0 ,sizeof(cmd));
            sprintf(cmd, "acs_cli2 -i wl%d autochannel &", index);
            system(cmd);
        }
        else {
            /* Set acsd2 disabled mode */
            wifi_hal_dbg_print("%s():%d Disabling autoChannel in radio index %d\n", __FUNCTION__, __LINE__, index);
            sprintf(cmd, "acs_cli2 -i wl%d mode 0 &", index);
            system(cmd);
        }
    }
    return 0;
}

int platform_post_init(wifi_vap_info_map_t *vap_map)
{
    int i, index;
    char param_name[NVRAM_NAME_SIZE];
    char interface_name[8];

    memset(param_name, 0 ,sizeof(param_name));
    memset(interface_name, 0, sizeof(interface_name));

    wifi_hal_info_print("%s:%d: start_wifi_apps\n", __func__, __LINE__);
    system("wifi_setup.sh start_wifi_apps");

    wifi_hal_dbg_print("%s:%d: add wifi interfaces to flow manager\r\n", __func__, __LINE__);
    system("wifi_setup.sh add_ifaces_to_flowmgr");

    if (system("killall -q -9 acsd2 2>/dev/null")) {
        wifi_hal_info_print("%s: system kill acsd2 failed\n", __FUNCTION__);
    }

    if (system("acsd2")) {
        wifi_hal_info_print("%s: system acsd2 failed\n", __FUNCTION__);
    }

#if defined(WLDM_21_2)
    wlcsm_nvram_set("acsd2_started", "1");
#else
    nvram_set("acsd2_started", "1");
#endif // defined(WLDM_21_2)

    wifi_hal_info_print("%s:%d: acsd2_started\r\n", __func__, __LINE__);

    //set runtime configs using wl command.
    set_wl_runtime_configs(vap_map);

    wifi_hal_dbg_print("%s:%d: wifi param set success\r\n", __func__, __LINE__);

    if (vap_map != NULL) {
        for(i = 0; i < g_wifi_hal.num_radios; i++) {
            if (vap_map != NULL) {
                for (index = 0; index < vap_map->num_vaps; index++) {
                    memset(param_name, 0 ,sizeof(param_name));
                    memset(interface_name, 0, sizeof(interface_name));
                    get_ccspwifiagent_interface_name_from_vap_index(vap_map->vap_array[index].vap_index, interface_name);
                    if (vap_map->vap_array[index].vap_mode == wifi_vap_mode_ap) {
                        prepare_param_name(param_name, interface_name, "_bss_maxassoc");
                        set_decimal_nvram_param(param_name, vap_map->vap_array[index].u.bss_info.bssMaxSta);
                        wifi_hal_dbg_print("%s:%d: nvram param name:%s vap_bssMaxSta:%d\r\n", __func__, __LINE__, param_name, vap_map->vap_array[index].u.bss_info.bssMaxSta);
                    }
                }
                vap_map++;
            } else {
                wifi_hal_error_print("%s:%d: vap_map NULL for radio_index:%d\r\n", __func__, __LINE__, i);
            }
        }
    }

    return 0;
}

int nvram_get_vap_enable_status(bool *vap_enable, int vap_index)
{
    char interface_name[10];
    char nvram_name[NVRAM_NAME_SIZE];

    memset(interface_name, 0, sizeof(interface_name));
    get_ccspwifiagent_interface_name_from_vap_index(vap_index, interface_name);

    snprintf(nvram_name, sizeof(nvram_name), "%s_vap_enabled", interface_name);
#if defined(WLDM_21_2)
    char *enable = wlcsm_nvram_get(nvram_name);
#else
    char *enable = nvram_get(nvram_name);
#endif // defined(WLDM_21_2)

    *vap_enable = (!enable || *enable == '0') ? FALSE : TRUE;
    wifi_hal_dbg_print("%s:%d: vap enable status:%d for vap index:%d \r\n", __func__, __LINE__, *vap_enable, vap_index);

    return 0;
}

int nvram_get_current_security_mode(wifi_security_modes_t *security_mode,int vap_index)
{
    char nvram_name[NVRAM_NAME_SIZE];
    char interface_name[8];
    char *sec_mode_str, *mfp_str;
    wifi_security_modes_t current_security_mode;

    memset(interface_name, 0, sizeof(interface_name));
    get_ccspwifiagent_interface_name_from_vap_index(vap_index, interface_name);
    snprintf(nvram_name, sizeof(nvram_name), "%s_akm", interface_name);
#if defined(WLDM_21_2)
    sec_mode_str = wlcsm_nvram_get(nvram_name);
#else
    sec_mode_str = nvram_get(nvram_name);
#endif // defined(WLDM_21_2)
    if (sec_mode_str == NULL) {
        wifi_hal_error_print("%s:%d nvram sec_mode value is NULL\r\n", __func__, __LINE__);
        return -1;
    }
    snprintf(nvram_name, sizeof(nvram_name), "%s_mfp", interface_name);
#if defined(WLDM_21_2)
    mfp_str = wlcsm_nvram_get(nvram_name);
#else
    mfp_str = nvram_get(nvram_name);
#endif // defined(WLDM_21_2)
    if (mfp_str == NULL) {
        wifi_hal_error_print("%s:%d nvram mfp value is NULL\r\n", __func__, __LINE__);
        return -1;
    }

    if (get_security_mode_int_from_str(sec_mode_str,mfp_str, &current_security_mode) == 0) {
        *security_mode = current_security_mode;
        return 0;
    }

    return -1;
}

int nvram_get_default_password(char *l_password, int vap_index)
{
    char nvram_name[NVRAM_NAME_SIZE];
    char interface_name[8];
    int len;
    char *key_passphrase;

    memset(interface_name, 0, sizeof(interface_name));
    get_ccspwifiagent_interface_name_from_vap_index(vap_index, interface_name);
    snprintf(nvram_name, sizeof(nvram_name), "%s_wpa_psk", interface_name);
#if defined(WLDM_21_2)
    key_passphrase = wlcsm_nvram_get(nvram_name);
#else
    key_passphrase = nvram_get(nvram_name);
#endif // defined(WLDM_21_2)

    if (key_passphrase == NULL) {
        wifi_hal_error_print("%s:%d nvram key_passphrase value is NULL\r\n", __func__, __LINE__);
        return -1;
    }
    len = strlen(key_passphrase);
    if (len < 8 || len > 63) {
        wifi_hal_error_print("%s:%d invalid wpa passphrase length [%d], expected length is [8..63]\r\n", __func__, __LINE__, len);
        return -1;
    }
    strncpy(l_password, key_passphrase, (len + 1));
    return 0;
}

int nvram_get_default_xhs_password(char *l_password, int vap_index)
{
    char nvram_name[NVRAM_NAME_SIZE];
    int len;
    char *key_passphrase;

    snprintf(nvram_name, sizeof(nvram_name), "xhs_wpa_psk");
#if defined(WLDM_21_2)
    key_passphrase = wlcsm_nvram_get(nvram_name);
#else
    key_passphrase = nvram_get(nvram_name);
#endif // defined(WLDM_21_2)

    if (key_passphrase == NULL) {
        wifi_hal_error_print("%s:%d nvram key_passphrase value is NULL\r\n", __func__, __LINE__);
        return -1;
    }
    len = strlen(key_passphrase);
    if (len < 8 || len > 63) {
        wifi_hal_error_print("%s:%d invalid wpa passphrase length [%d], expected length is [8..63]\r\n", __func__, __LINE__, len);
        return -1;
    }
    strncpy(l_password, key_passphrase, (len + 1));
    return 0;
}

int platform_get_keypassphrase_default(char *password, int vap_index)
{
    char value[BUFFER_LENGTH_WIFIDB] = {0};
    FILE *fp = NULL;

    if(is_wifi_hal_vap_private(vap_index)) {
#if defined(SKYSR300_PORT) || defined(SKYSR213_PORT)
        fp = popen("grep \"WIFIPASSWORD=\" /tmp/serial.txt | cut -d '=' -f 2 | tr -d '\r\n'","r");
#else
        fp = popen("grep \"Default WIFI Password:\" /tmp/factory_nvram.data | cut -d ':' -f2 | cut -d ' ' -f2","r");
#endif
        if(fp != NULL) {
            while (fgets(value, sizeof(value), fp) != NULL){
#if defined(SKYSR300_PORT) || defined(SKYSR213_PORT)
                strncpy(password,value,strlen(value));
#else
                strncpy(password,value,strlen(value)-1);
#endif
            }
            pclose(fp);
            return 0;
        }
    } else if(is_wifi_hal_vap_xhs(vap_index)) {
#if defined(TCXB7_PORT) || defined(TCXB8_PORT) || defined(XB10_PORT)
        return nvram_get_default_xhs_password(password, vap_index);
#else
        return nvram_get_default_password(password, vap_index);
#endif
    } else {
        return nvram_get_default_password(password, vap_index);
    }
    return -1;
}
int platform_get_radius_key_default(char *radius_key)
{
    char nvram_name[NVRAM_NAME_SIZE];
    char *key;

    snprintf(nvram_name, sizeof(nvram_name), "default_radius_key");
#if defined(WLDM_21_2)
    key = wlcsm_nvram_get(nvram_name);
#else
    key = nvram_get(nvram_name);
#endif // defined(WLDM_21_2)
    if (key == NULL) {
        wifi_hal_error_print("%s:%d default_radius_key value is NULL\r\n", __func__, __LINE__);
        return -1;
    }
    else {
        strncpy(radius_key, key, (strlen(key) + 1));
    }
    return 0;
}
int platform_get_ssid_default(char *ssid, int vap_index){
    char value[BUFFER_LENGTH_WIFIDB] = {0};
    FILE *fp = NULL;

    if(is_wifi_hal_vap_private(vap_index)) {

#if defined(SKYSR300_PORT) || defined(SKYSR213_PORT)
        fp = popen("grep \"FACTORYSSID=\" /tmp/serial.txt | cut -d '=' -f2 | tr -d '\r\n'","r");
#else
        fp = popen("grep \"Default 2.4 GHz SSID:\" /tmp/factory_nvram.data | cut -d ':' -f2 | cut -d ' ' -f2","r");
#endif

        if(fp != NULL) {
            while (fgets(value, sizeof(value), fp) != NULL){
#if defined(SKYSR300_PORT) || defined(SKYSR213_PORT)
                strncpy(ssid,value,strlen(value));
#else
                strncpy(ssid,value,strlen(value)-1);
#endif
            }
            pclose(fp);
            return 0;
        }
    } else if(is_wifi_hal_vap_xhs(vap_index)) {
#if defined(TCXB7_PORT) || defined(TCXB8_PORT) || defined(XB10_PORT)
        return nvram_get_default_xhs_ssid(ssid, vap_index);
#else
        return nvram_get_current_ssid(ssid, vap_index);
#endif
    } else {
        return nvram_get_current_ssid(ssid, vap_index);
    }
    return -1;
}

int platform_get_wps_pin_default(char *pin)
{
    char value[BUFFER_LENGTH_WIFIDB] = {0};
    FILE *fp = NULL;
#if defined(SKYSR300_PORT) || defined(SKYSR213_PORT)
    fp = popen("grep \"WPSPIN=\" /tmp/serial.txt | cut -d '=' -f2 | tr -d '\r\n'","r");
#else
    fp = popen("grep \"Default WPS Pin:\" /tmp/factory_nvram.data | cut -d ':' -f2 | cut -d ' ' -f2","r");
#endif
    if(fp != NULL) {
        while (fgets(value, sizeof(value), fp) != NULL) {
#if defined(SKYSR300_PORT) || defined(SKYSR213_PORT)
            strncpy(pin,value,strlen(value));
#else
            strncpy(pin,value,strlen(value)-1);
#endif
        }
        pclose(fp);
        return 0;
    }
    return -1;
}

int platform_wps_event(wifi_wps_event_t data)
{
    switch(data.event) {
        case WPS_EV_PBC_ACTIVE:
        case WPS_EV_PIN_ACTIVE:
#if defined(_SR213_PRODUCT_REQ_) && defined(FEATURE_RDKB_LED_MANAGER)
            // set led to blinking blue
            system("sysevent set led_event rdkb_wps_start");
            wifi_hal_dbg_print("%s:%d set wps led color to blinking blue \r\n", __func__, __LINE__);
#else
            // set wps led color to blue
            system("led_wps_active 1");
            wifi_hal_dbg_print("%s:%d set wps led color to blue\r\n", __func__, __LINE__);
#endif // defined(_SR213_PRODUCT_REQ_) && defined(FEATURE_RDKB_LED_MANAGER)
            break;

        case WPS_EV_SUCCESS:
        case WPS_EV_PBC_TIMEOUT:
        case WPS_EV_PIN_TIMEOUT:
        case WPS_EV_PIN_DISABLE:
        case WPS_EV_PBC_DISABLE:
#if defined(_SR213_PRODUCT_REQ_) && defined(FEATURE_RDKB_LED_MANAGER)
            system("sysevent set led_event rdkb_wps_stop");
            wifi_hal_dbg_print("%s:%d set wps led color to solid white \r\n", __func__, __LINE__);
#else
            // set wps led color to white
            system("led_wps_active 0");
            wifi_hal_dbg_print("%s:%d set wps led color to white\r\n", __func__, __LINE__);
#endif //defined(_SR213_PRODUCT_REQ_) && defined(FEATURE_RDKB_LED_MANAGER)
            break;

        default:
            wifi_hal_info_print("%s:%d wps event[%d] not handle\r\n", __func__, __LINE__, data.event);
            break;
    }

    return 0;
}

int platform_get_country_code_default(char *code)
{
    char value[BUFFER_LENGTH_WIFIDB] = {0};
    FILE *fp = NULL;

#if defined(SKYSR300_PORT) || defined(SKYSR213_PORT)
    fp = popen("grep \"REGION=\" /tmp/serial.txt | cut -d '=' -f 2 | tr -d '\r\n'","r");
#else
    fp = popen("cat /data/.customerId", "r");
#endif

    if (fp != NULL) {
        while(fgets(value, sizeof(value), fp) != NULL) {
#if defined(SKYSR300_PORT) || defined(SKYSR213_PORT)
            strncpy(code, value, strlen(value));
#else
            strncpy(code, value, strlen(value)-1);
#endif
        }
        pclose(fp);
        return 0;
    }
    return -1;
}

int nvram_get_current_password(char *l_password, int vap_index)
{
    return nvram_get_default_password(l_password, vap_index);
}

int nvram_get_current_ssid(char *l_ssid, int vap_index)
{
    char nvram_name[NVRAM_NAME_SIZE];
    char interface_name[8];
    int len;
    char *ssid;

    memset(interface_name, 0, sizeof(interface_name));
    get_ccspwifiagent_interface_name_from_vap_index(vap_index, interface_name);
    snprintf(nvram_name, sizeof(nvram_name), "%s_ssid", interface_name);
#if defined(WLDM_21_2)
    ssid = wlcsm_nvram_get(nvram_name);
#else
    ssid = nvram_get(nvram_name);
#endif // defined(WLDM_21_2)
    if (ssid == NULL) {
        wifi_hal_error_print("%s:%d nvram ssid value is NULL\r\n", __func__, __LINE__);
        return -1;
    }
    len = strlen(ssid);
    if (len < 0 || len > 63) {
        wifi_hal_error_print("%s:%d invalid ssid length [%d], expected length is [0..63]\r\n", __func__, __LINE__, len);
        return -1;
    }
    strncpy(l_ssid, ssid, (len + 1));
    wifi_hal_dbg_print("%s:%d vap[%d] ssid:%s nvram name:%s\r\n", __func__, __LINE__, vap_index, l_ssid, nvram_name);
    return 0;
}

int nvram_get_default_xhs_ssid(char *l_ssid, int vap_index)
{
    char nvram_name[NVRAM_NAME_SIZE];
    int len;
    char *ssid;

    snprintf(nvram_name, sizeof(nvram_name), "xhs_ssid");
#if defined(WLDM_21_2)
    ssid = wlcsm_nvram_get(nvram_name);
#else
    ssid = nvram_get(nvram_name);
#endif // defined(WLDM_21_2)
    if (ssid == NULL) {
        wifi_hal_error_print("%s:%d nvram ssid value is NULL\r\n", __func__, __LINE__);
        return -1;
    }
    len = strlen(ssid);
    if (len < 0 || len > 63) {
        wifi_hal_error_print("%s:%d invalid ssid length [%d], expected length is [0..63]\r\n", __func__, __LINE__, len);
        return -1;
    }
    strncpy(l_ssid, ssid, (len + 1));
    wifi_hal_dbg_print("%s:%d vap[%d] ssid:%s nvram name:%s\r\n", __func__, __LINE__, vap_index, l_ssid, nvram_name);
    return 0;
}

static int get_control_side_band(wifi_radio_index_t index, wifi_radio_operationParam_t *operationParam)
{
    wifi_radio_info_t *radio;
    int sec_chan_offset, freq;
    char country[8];

    radio = get_radio_by_rdk_index(index);
    get_coutry_str_from_code(operationParam->countryCode, country);

    freq = ieee80211_chan_to_freq(country, operationParam->op_class, operationParam->channel);
    sec_chan_offset = get_sec_channel_offset(radio, freq);

    return sec_chan_offset;
}

static char *channel_width_to_string_convert(wifi_channelBandwidth_t channelWidth)
{
    switch(channelWidth)
    {
    case WIFI_CHANNELBANDWIDTH_20MHZ:
        return "20";
    case WIFI_CHANNELBANDWIDTH_40MHZ:
        return "40";
    case WIFI_CHANNELBANDWIDTH_80MHZ:
        return "80";
    case WIFI_CHANNELBANDWIDTH_160MHZ:
        return "160";
    case WIFI_CHANNELBANDWIDTH_80_80MHZ:
    default:
        return NULL;
    }
}

static int get_chanspec_string(wifi_radio_operationParam_t *operationParam, char *chspec, wifi_radio_index_t index)
{
    char *sideband = "";
    char *band = "";
    char *bw = NULL;

    if (operationParam->band != WIFI_FREQUENCY_2_4_BAND) {
        bw = channel_width_to_string_convert(operationParam->channelWidth);
        if (bw == NULL) {
            wifi_hal_error_print("%s:%d: Channel width %d not supported in radio index: %d\n", __func__, __LINE__, operationParam->channelWidth, index);
            return -1;
        }
    }

    if (operationParam->band == WIFI_FREQUENCY_6_BAND) {
        band = "6g";
    }
    if (operationParam->channelWidth == WIFI_CHANNELBANDWIDTH_20MHZ) {
        sprintf(chspec, "%s%d", band, operationParam->channel);
    }
    else if ((operationParam->channelWidth == WIFI_CHANNELBANDWIDTH_40MHZ) && (operationParam->band != WIFI_FREQUENCY_6_BAND)) {
        sideband = (get_control_side_band(index, operationParam)) == 1 ? "l" : "u";
        sprintf(chspec, "%d%s", operationParam->channel, sideband);
    }
    else {
        sprintf(chspec, "%s%d/%s", band, operationParam->channel, bw);
    }
    return 0;
}

int platform_set_radio(wifi_radio_index_t index, wifi_radio_operationParam_t *operationParam)
{
    char temp_buff[BUF_SIZE];
    char param_name[NVRAM_NAME_SIZE];
    char chspecbuf[NVRAM_NAME_SIZE];
    memset(chspecbuf, 0 ,sizeof(chspecbuf));
    memset(param_name, 0 ,sizeof(param_name));
    memset(temp_buff, 0 ,sizeof(temp_buff));
    wifi_hal_dbg_print("%s:%d: Enter radio index:%d\n", __func__, __LINE__, index);

    memset(param_name, 0 ,sizeof(param_name));
    sprintf(param_name, "wl%d_auto_cha", index);
    set_decimal_nvram_param(param_name, operationParam->autoChannelEnabled);

    if (operationParam->autoChannelEnabled) {
        set_string_nvram_param("acsd_restart", "yes");
        memset(param_name, 0 ,sizeof(param_name));
        sprintf(param_name, "wl%d_channel", index);
        set_decimal_nvram_param(param_name, 0);

        memset(param_name, 0 ,sizeof(param_name));
        sprintf(param_name, "wl%d_chanspec", index);
        set_decimal_nvram_param(param_name, 0);
    } else {
        memset(param_name, 0 ,sizeof(param_name));
        sprintf(param_name, "wl%d_channel", index);
        set_decimal_nvram_param(param_name, operationParam->channel);

        get_chanspec_string(operationParam, chspecbuf, index);
        memset(param_name, 0 ,sizeof(param_name));
        sprintf(param_name, "wl%d_chanspec", index);
        set_string_nvram_param(param_name, chspecbuf);
    }

    memset(param_name, 0 ,sizeof(param_name));
    sprintf(param_name, "wl%d_dtim", index);
    set_decimal_nvram_param(param_name, operationParam->dtimPeriod);

    memset(param_name, 0 ,sizeof(param_name));
    sprintf(param_name, "wl%d_frag", index);
    set_decimal_nvram_param(param_name, operationParam->fragmentationThreshold);

    memset(param_name, 0 ,sizeof(param_name));
    sprintf(param_name, "wl%d_nband", index);
    set_decimal_nvram_param(param_name, operationParam->band);

    memset(param_name, 0 ,sizeof(param_name));
    memset(temp_buff, 0 ,sizeof(temp_buff));
    sprintf(param_name, "wl%d_oper_stands", index);
    get_radio_variant_str_from_int(operationParam->variant, temp_buff);
    set_string_nvram_param(param_name, temp_buff);

    memset(param_name, 0 ,sizeof(param_name));
    sprintf(param_name, "wl%d_bcn", index);
    set_decimal_nvram_param(param_name, operationParam->beaconInterval);

    return 0;
}

int platform_create_vap(wifi_radio_index_t r_index, wifi_vap_info_map_t *map)
{
    wifi_hal_dbg_print("%s:%d: Enter radio index:%d\n", __func__, __LINE__, r_index);
    int  index = 0, l_wps_state = 0;
    char temp_buff[256];
    char param_name[NVRAM_NAME_SIZE];
    char interface_name[8];
    wifi_radio_info_t *radio;
    char das_ipaddr[45];
    memset(temp_buff, 0 ,sizeof(temp_buff));
    memset(param_name, 0 ,sizeof(param_name));
    memset(interface_name, 0, sizeof(interface_name));

    for (index = 0; index < map->num_vaps; index++) {

        radio = get_radio_by_rdk_index(r_index);
        if (radio == NULL) {
            wifi_hal_error_print("%s:%d:Could not find radio index:%d\n", __func__, __LINE__, r_index);
            return RETURN_ERR;
        }

        memset(interface_name, 0, sizeof(interface_name));
        get_ccspwifiagent_interface_name_from_vap_index(map->vap_array[index].vap_index, interface_name);

        prepare_param_name(param_name, interface_name, "_ifname");
        set_string_nvram_param(param_name, interface_name);

        memset(temp_buff, 0 ,sizeof(temp_buff));
        prepare_param_name(param_name, interface_name, "_mode");
        get_vap_mode_str_from_int_mode(map->vap_array[index].vap_mode, temp_buff);
        set_string_nvram_param(param_name, temp_buff);

        prepare_param_name(param_name, interface_name, "_radio");
        set_decimal_nvram_param(param_name, 1);

        prepare_param_name(param_name, interface_name, "_reg_mode");
        if (radio->iconf.ieee80211h != 0) {
            set_string_nvram_param(param_name, "h");
        } else {
            set_decimal_nvram_param(param_name, radio->iconf.ieee80211h);
        }

        if (map->vap_array[index].vap_mode == wifi_vap_mode_ap) {

            prepare_param_name(param_name, interface_name, "_akm");
            memset(temp_buff, 0 ,sizeof(temp_buff));
            if (get_security_mode_str_from_int(map->vap_array[index].u.bss_info.security.mode, temp_buff) == RETURN_OK) {
                set_string_nvram_param(param_name, temp_buff);
            }

            prepare_param_name(param_name, interface_name, "_crypto");
            memset(temp_buff, 0 ,sizeof(temp_buff));
            if (get_security_encryption_mode_str_from_int(map->vap_array[index].u.bss_info.security.encr, temp_buff) == RETURN_OK) {
                set_string_nvram_param(param_name, temp_buff);
            }

            prepare_param_name(param_name, interface_name, "_mfp");
            set_decimal_nvram_param(param_name, map->vap_array[index].u.bss_info.security.mfp);

            prepare_param_name(param_name, interface_name, "_ap_isolate");
            set_decimal_nvram_param(param_name, map->vap_array[index].u.bss_info.isolation);

            prepare_param_name(param_name, interface_name, "_vap_enabled");
            set_decimal_nvram_param(param_name, map->vap_array[index].u.bss_info.enabled);

            prepare_param_name(param_name, interface_name, "_closed");
            set_decimal_nvram_param(param_name, !map->vap_array[index].u.bss_info.showSsid);

            prepare_param_name(param_name, interface_name, "_bss_maxassoc");
            set_decimal_nvram_param(param_name, map->vap_array[index].u.bss_info.bssMaxSta);

            /*
             * RDKB-52611:
             * Call API to populate the 'bssMaxSta' value in driver context (wl) for corresponding VAP index.
             */
            wifi_setApMaxAssociatedDevices(map->vap_array[index].vap_index, map->vap_array[index].u.bss_info.bssMaxSta);

            if (strlen(map->vap_array[index].repurposed_vap_name) == 0) {
                prepare_param_name(param_name, interface_name, "_ssid");
                set_string_nvram_param(param_name, map->vap_array[index].u.bss_info.ssid);
            } else {
                wifi_hal_info_print("%s is repurposed to %s hence not setting in nvram \n",map->vap_array[index].vap_name,map->vap_array[index].repurposed_vap_name);
            }

            memset(temp_buff, 0 ,sizeof(temp_buff));
            prepare_param_name(param_name, interface_name, "_wps_mode");
            if (map->vap_array[index].u.bss_info.wps.enable) {
                strcpy(temp_buff, "enabled");
            } else {
                strcpy(temp_buff, "disabled");
            }
            set_string_nvram_param(param_name, temp_buff);

            prepare_param_name(param_name, interface_name, "_wps_device_pin");
            set_string_nvram_param(param_name, map->vap_array[index].u.bss_info.wps.pin);

            memset(temp_buff, 0 ,sizeof(temp_buff));
            prepare_param_name(param_name, interface_name, "_wps_method_enabled");
            wps_enum_to_string(map->vap_array[index].u.bss_info.wps.methods, temp_buff, sizeof(temp_buff));
            set_string_nvram_param(param_name, temp_buff);

            l_wps_state = map->vap_array[index].u.bss_info.wps.enable ? WPS_STATE_CONFIGURED : 0;
            /* WPS is not supported in 6G */
            if (radio->oper_param.band == WIFI_FREQUENCY_6_BAND) {
                l_wps_state = 0;
            }
            if (l_wps_state && (!map->vap_array[index].u.bss_info.showSsid)) {
                l_wps_state = 0;
            }
            prepare_param_name(param_name, interface_name, "_wps_config_state");
            set_decimal_nvram_param(param_name, l_wps_state);

            if ((get_security_mode_support_radius(map->vap_array[index].u.bss_info.security.mode))|| is_wifi_hal_vap_hotspot_open(map->vap_array[index].vap_index)) {

                prepare_param_name(param_name, interface_name, "_radius_port");
                set_decimal_nvram_param(param_name, map->vap_array[index].u.bss_info.security.u.radius.port);

                prepare_param_name(param_name, interface_name, "_radius_ipaddr");
                set_string_nvram_param(param_name, map->vap_array[index].u.bss_info.security.u.radius.ip);

                prepare_param_name(param_name, interface_name, "_radius_key");
                set_string_nvram_param(param_name, map->vap_array[index].u.bss_info.security.u.radius.key);

                prepare_param_name(param_name, interface_name, "_radius2_port");
                set_decimal_nvram_param(param_name, map->vap_array[index].u.bss_info.security.u.radius.s_port);

                prepare_param_name(param_name, interface_name, "_radius2_ipaddr");
                set_string_nvram_param(param_name, map->vap_array[index].u.bss_info.security.u.radius.s_ip);

                prepare_param_name(param_name, interface_name, "_radius2_key");
                set_string_nvram_param(param_name, map->vap_array[index].u.bss_info.security.u.radius.s_key);

                memset(&das_ipaddr, 0, sizeof(das_ipaddr));
                getIpStringFromAdrress(das_ipaddr,&map->vap_array[index].u.bss_info.security.u.radius.dasip);

                prepare_param_name(param_name, interface_name, "_radius_das_client_ipaddr");
                set_string_nvram_param(param_name, das_ipaddr);

                prepare_param_name(param_name, interface_name, "_radius_das_key");
                set_string_nvram_param(param_name, map->vap_array[index].u.bss_info.security.u.radius.daskey);

                prepare_param_name(param_name, interface_name, "_radius_das_port");
                set_decimal_nvram_param(param_name, map->vap_array[index].u.bss_info.security.u.radius.dasport);
            } else {

                if (strlen(map->vap_array[index].repurposed_vap_name) == 0) {
                    prepare_param_name(param_name, interface_name, "_wpa_psk");
                    set_string_nvram_param(param_name, map->vap_array[index].u.bss_info.security.u.key.key);
                } else {
                    wifi_hal_info_print("%s is repurposed to %s hence not setting in nvram \n",map->vap_array[index].vap_name,map->vap_array[index].repurposed_vap_name);
                }
            }

            prepare_param_name(param_name, interface_name, "_hessid");
            set_string_nvram_param(param_name, map->vap_array[index].u.bss_info.interworking.interworking.hessid);

            prepare_param_name(param_name, interface_name, "_venuegrp");
            set_decimal_nvram_param(param_name, map->vap_array[index].u.bss_info.interworking.interworking.venueGroup);

            prepare_param_name(param_name, interface_name, "_venuetype");
            set_decimal_nvram_param(param_name, map->vap_array[index].u.bss_info.interworking.interworking.venueType);
    
            prepare_param_name(param_name, interface_name, "_bcnprs_txpwr_offset");
            set_decimal_nvram_param(param_name, abs(map->vap_array[index].u.bss_info.mgmtPowerControl));

        } else if (map->vap_array[index].vap_mode == wifi_vap_mode_sta) {

            prepare_param_name(param_name, interface_name, "_akm");
            memset(temp_buff, 0 ,sizeof(temp_buff));
            if (get_security_mode_str_from_int(map->vap_array[index].u.sta_info.security.mode, temp_buff) == RETURN_OK) {
                set_string_nvram_param(param_name, temp_buff);
            }

            prepare_param_name(param_name, interface_name, "_crypto");
            memset(temp_buff, 0 ,sizeof(temp_buff));
            if (get_security_encryption_mode_str_from_int(map->vap_array[index].u.sta_info.security.encr, temp_buff) == RETURN_OK) {
                set_string_nvram_param(param_name, temp_buff);
            }

            prepare_param_name(param_name, interface_name, "_mfp");
            set_decimal_nvram_param(param_name, map->vap_array[index].u.sta_info.security.mfp);

            prepare_param_name(param_name, interface_name, "_ssid");
            set_string_nvram_param(param_name, map->vap_array[index].u.sta_info.ssid);


            if ((get_security_mode_support_radius(map->vap_array[index].u.sta_info.security.mode))|| is_wifi_hal_vap_hotspot_open(map->vap_array[index].vap_index)) {

                prepare_param_name(param_name, interface_name, "_radius_port");
                set_decimal_nvram_param(param_name, map->vap_array[index].u.sta_info.security.u.radius.port);

                prepare_param_name(param_name, interface_name, "_radius_ipaddr");
                set_string_nvram_param(param_name, map->vap_array[index].u.sta_info.security.u.radius.ip);

                prepare_param_name(param_name, interface_name, "_radius_key");
                set_string_nvram_param(param_name, map->vap_array[index].u.sta_info.security.u.radius.key);

                prepare_param_name(param_name, interface_name, "_radius2_port");
                set_decimal_nvram_param(param_name, map->vap_array[index].u.sta_info.security.u.radius.s_port);

                prepare_param_name(param_name, interface_name, "_radius2_ipaddr");
                set_string_nvram_param(param_name, map->vap_array[index].u.sta_info.security.u.radius.s_ip);

                prepare_param_name(param_name, interface_name, "_radius2_key");
                set_string_nvram_param(param_name, map->vap_array[index].u.sta_info.security.u.radius.s_key);

                memset(&das_ipaddr, 0, sizeof(das_ipaddr));
                getIpStringFromAdrress(das_ipaddr,&map->vap_array[index].u.sta_info.security.u.radius.dasip);

                prepare_param_name(param_name, interface_name, "_radius_das_client_ipaddr");
                set_string_nvram_param(param_name, das_ipaddr);

                prepare_param_name(param_name, interface_name, "_radius_das_key");
                set_string_nvram_param(param_name, map->vap_array[index].u.sta_info.security.u.radius.daskey);

                prepare_param_name(param_name, interface_name, "_radius_das_port");
                set_decimal_nvram_param(param_name, map->vap_array[index].u.sta_info.security.u.radius.dasport);

            } else {
                prepare_param_name(param_name, interface_name, "_wpa_psk");
                set_string_nvram_param(param_name, map->vap_array[index].u.sta_info.security.u.key.key);
            }
        }
    }

    return 0;
}

int platform_pre_create_vap(wifi_radio_index_t index, wifi_vap_info_map_t *map)
{
#if defined(_SR213_PRODUCT_REQ_)
    char interface_name[10];
    char param[128];
    wifi_vap_info_t *vap;
    unsigned int vap_itr = 0;

    for (vap_itr=0; vap_itr < map->num_vaps; vap_itr++) {
        memset(interface_name, 0, sizeof(interface_name));
        memset(param, 0, sizeof(param));
        vap = &map->vap_array[vap_itr];
        get_interface_name_from_vap_index(vap->vap_index, interface_name);
        snprintf(param, sizeof(param), "%s_bss_enabled", interface_name);
        if (vap->vap_mode == wifi_vap_mode_ap) {
            if (vap->u.bss_info.enabled) {
#if defined(WLDM_21_2)
                wlcsm_nvram_set(param, "1");
#else
                nvram_set(param, "1");
#endif // defined(WLDM_21_2)
            }else {
#if defined(WLDM_21_2)
                wlcsm_nvram_set(param, "0");
#else
                nvram_set(param, "0");
#endif // defined(WLDM_21_2)
            }
        }else if (vap->vap_mode == wifi_vap_mode_sta) {
            if (vap->u.sta_info.enabled) {
#if defined(WLDM_21_2)
                wlcsm_nvram_set(param, "1");
#else
                nvram_set(param, "1");
#endif // defined(WLDM_21_2)
            } else {
#if defined(WLDM_21_2)
                wlcsm_nvram_set(param, "0");
#else
                nvram_set(param, "0");
#endif // defined(WLDM_21_2)
            }
        }
    }
#endif //defined(_SR213_PRODUCT_REQ_)
    return 0;
}

int wifi_setQamPlus(void *priv)
{
    return 0;
}

int wifi_setApRetrylimit(void *priv)
{
    return 0;
}

int platform_flags_init(int *flags)
{
    return 0;
}

int platform_get_aid(void* priv, u16* aid, const u8* addr)
{
    return 0;
}

int platform_free_aid(void* priv, u16* aid)
{
    return 0;
}

int platform_sync_done(void* priv)
{
    return 0;
}

int platform_get_channel_bandwidth(wifi_radio_index_t index,  wifi_channelBandwidth_t *channelWidth)
{
    return 0;
}

int platform_update_radio_presence(void)
{
    char cmd[32] = {0};
    unsigned int index = 0, value = 0;
    wifi_radio_info_t *radio;
    char buf[2] = {0};
    FILE *fp = NULL;

    wifi_hal_error_print("%s:%d: g_wifi_hal.num_radios %d\n", __func__, __LINE__, g_wifi_hal.num_radios);

    for (index = 0; index < g_wifi_hal.num_radios; index++)
    {
       radio = get_radio_by_rdk_index(index);
       snprintf(cmd, sizeof(cmd), "nvram kget wl%d_dpd", index);
       if ((fp = popen(cmd, "r")) != NULL)
       {
           if (fgets(buf, sizeof(buf), fp) != NULL)
           {
               value = atoi(buf);
               if (1 == value) {
                   radio->radio_presence = false;
               }
               wifi_hal_info_print("%s:%d: Index %d edpd enable %d presence %d\n", __func__, __LINE__, index, value, radio->radio_presence);
           }
           pclose(fp);
       }
    }
    return 0;
}

int platform_get_acl_num(int vap_index, uint *acl_count)
{
    return 0;
}

int nvram_get_mgmt_frame_power_control(int vap_index, int* output_dbm)
{
    char nvram_name[NVRAM_NAME_SIZE];
    char interface_name[8];
    char *str_value;

    if (output_dbm == NULL) {
        wifi_hal_error_print("%s:%d - Null output buffer\n", __func__, __LINE__);
        return RETURN_ERR;
    }

    memset(interface_name, 0, sizeof(interface_name));
    get_ccspwifiagent_interface_name_from_vap_index(vap_index, interface_name);
    snprintf(nvram_name, sizeof(nvram_name), "%s_bcnprs_txpwr_offset", interface_name);
#if defined(WLDM_21_2)
    str_value = wlcsm_nvram_get(nvram_name);
#else
    str_value = nvram_get(nvram_name);
#endif // defined(WLDM_21_2)
    if (str_value == NULL) {
        wifi_hal_error_print("%s:%d nvram %s value is NULL\r\n", __func__, __LINE__, nvram_name);
        return RETURN_ERR;
    }

    *output_dbm = 0 - atoi(str_value);
    wifi_hal_dbg_print("%s:%d - MFPC for VAP %d is %d\n", __func__, __LINE__, vap_index, *output_dbm);
    return RETURN_OK;
}

#if defined(_SR213_PRODUCT_REQ_)

int platform_get_radio_phytemperature(wifi_radio_index_t index,
    wifi_radioTemperature_t *radioPhyTemperature)
{
    // no wl_iovar_getint API
    return RETURN_OK;
}

#endif // _SR213_PRODUCT_REQ_

#if defined(TCXB7_PORT) || defined(TCXB8_PORT) || defined(XB10_PORT)

static int get_radio_phy_temp_handler(struct nl_msg *msg, void *arg)
{
    int t;
    struct nlattr *nlattr;
    struct nlattr *tb[NL80211_ATTR_MAX + 1];
    struct nlattr *tb_vendor[RDK_VENDOR_ATTR_MAX + 1];
    static struct nla_policy vendor_policy[RDK_VENDOR_ATTR_MAX + 1] = {
        [RDK_VENDOR_ATTR_WIPHY_TEMP] = { .type = NLA_S32 },
    };
    struct genlmsghdr *gnlh = nlmsg_data(nlmsg_hdr(msg));
    unsigned int *temp = (unsigned int *)arg;

    if (nla_parse(tb, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0), genlmsg_attrlen(gnlh, 0),
        NULL) < 0) {
        wifi_hal_error_print("%s:%d Failed to parse vendor data\n", __func__, __LINE__);
        return NL_SKIP;
    }

    if (tb[NL80211_ATTR_VENDOR_DATA] == NULL) {
        wifi_hal_error_print("%s:%d Vendor data is missing\n", __func__, __LINE__);
        return NL_SKIP;
    }

    nlattr = tb[NL80211_ATTR_VENDOR_DATA];
    if (nla_parse(tb_vendor, RDK_VENDOR_ATTR_MAX, nla_data(nlattr), nla_len(nlattr),
        vendor_policy) < 0) {
        wifi_hal_error_print("%s:%d Failed to parse vendor attribute\n", __func__, __LINE__);
        return NL_SKIP;
    }

    if (tb_vendor[RDK_VENDOR_ATTR_WIPHY_TEMP] == NULL) {
        wifi_hal_error_print("%s:%d wiphy temp attribute is missing\n", __func__, __LINE__);
        return NL_SKIP;
    }

    t = nla_get_s32(tb_vendor[RDK_VENDOR_ATTR_WIPHY_TEMP]);
    *temp  = t >= 0 ? t : 0;

    return NL_SKIP;
}

static int get_radio_phy_temp(wifi_interface_info_t *interface, unsigned int *temp)
{
    struct nl_msg *msg;
    int ret = RETURN_ERR;

    msg = nl80211_drv_vendor_cmd_msg(g_wifi_hal.nl80211_id, interface, 0, OUI_COMCAST,
        RDK_VENDOR_NL80211_SUBCMD_GET_WIPHY_TEMP);
    if (msg == NULL) {
        wifi_hal_error_print("%s:%d Failed to create NL command\n", __func__, __LINE__);
        return RETURN_ERR;
    }

    ret = nl80211_send_and_recv(msg, get_radio_phy_temp_handler, temp, NULL, NULL);
    if (ret) {
        wifi_hal_error_print("%s:%d Failed to send NL message\n", __func__, __LINE__);
        return RETURN_ERR;
    }

    return RETURN_OK;
}

int platform_get_radio_phytemperature(wifi_radio_index_t index,
    wifi_radioTemperature_t *radioPhyTemperature)
{
    wifi_radio_info_t *radio;
    wifi_interface_info_t *interface;

    radio = get_radio_by_phy_index(index);
    if (radio == NULL) {
        wifi_hal_error_print("%s:%d: Failed to get radio for index: %d\n", __func__, __LINE__,
            index);
        return RETURN_ERR;
    }

    interface = get_primary_interface(radio);
    if (interface == NULL) {
        wifi_hal_error_print("%s:%d: Failed to get interface for radio index: %d\n", __func__,
            __LINE__, index);
        return RETURN_ERR;
    }

    if (get_radio_phy_temp(interface, &radioPhyTemperature->radio_Temperature)) {
        wifi_hal_error_print("%s:%d: Failed to get phy temperature for radio index: %d\n", __func__,
            __LINE__, index);
        return RETURN_ERR;
    }

    wifi_hal_dbg_print("%s:%d: radio index: %d temperature: %u\n", __func__, __LINE__, index,
        radioPhyTemperature->radio_Temperature);

    return RETURN_OK;
}

#elif defined (TCHCBRV2_PORT)

int platform_get_radio_phytemperature(wifi_radio_index_t index,
    wifi_radioTemperature_t *radioPhyTemperature)
{
    char ifname[32];

    snprintf(ifname, sizeof(ifname), "wl%d", index);
    if (wl_iovar_getint(ifname, "phy_tempsense", &radioPhyTemperature->radio_Temperature) < 0) {
        wifi_hal_error_print("%s:%d Failed to get temperature for radio: %d\n", __func__, __LINE__,
            index);
        return RETURN_ERR;
    }
    wifi_hal_dbg_print("%s:%d Temperature is %u\n", __func__, __LINE__, radioPhyTemperature->radio_Temperature);
    return RETURN_OK;
}
#endif // TCXB7_PORT || TCXB8_PORT || XB10_PORT

#if defined (ENABLED_EDPD) && defined(_SR213_PRODUCT_REQ_)
/* EDPD - WLAN Power down control support APIs. */
#define GPIO_PIN_24G_RADIO 101
#define GPIO_PIN_5G_RADIO 102
#define GPIO_EXPORT_PATH "/sys/class/gpio/export"
#define GPIO_UNEXPORT_PATH "/sys/class/gpio/unexport"
#define GPIO_DIRECTION_PATH "/sys/class/gpio/gpio%d/direction"
#define GPIO_VALUE_PATH "/sys/class/gpio/gpio%d/value"
#define ECOMODE_SCRIPT_FILE "/etc/sky/wifi.sh"
#define GPIO_DIRECTION_OUT "out"
#define BUFLEN_2 2

/**
 * @brief Enable EDPD ECO mode  feature control configuration
 */
static int enable_echo_feature_and_power_control_configs(void)
{
    if (check_edpdctl_enabled() && check_dpd_feature_enabled()) {
        wifi_hal_dbg_print("%s:%d: EDPD feature enabled in CPE\n", __func__, __LINE__);
        return RETURN_OK;
    }

    char cmd[BUFLEN_256] = {0};
    int rc = 0;

    snprintf(cmd, sizeof(cmd), "nvram kset wl_edpdctl_enable=1;nvram kcommit;nvram set wl_edpdctl_enable=1;nvram commit;sync");
    rc = system(cmd);
    if (rc == 0) {
        wifi_hal_dbg_print("%s:%d cmd [%s] successful \n", __func__, __LINE__, cmd);
    } else {
        wifi_hal_dbg_print("%s:%d cmd [%s] unsuccessful \n", __func__, __LINE__, cmd);
    }

    snprintf(cmd, sizeof(cmd), " /etc/sky/wifi.sh dpden 1");
    rc = system(cmd);
    if (rc == 0) {
        wifi_hal_dbg_print("%s:%d cmd [%s] successful \n", __func__, __LINE__, cmd);
    } else {
        wifi_hal_dbg_print("%s:%d cmd [%s] unsuccessful \n", __func__, __LINE__, cmd);
    }

    return rc;
}

/**
 * @brief API to check DPD feature enabled in CPE.
 *
 * @return int - Return 1 if feature enabled else returns 0.
 */
static int check_dpd_feature_enabled(void)
{
    FILE *fp = NULL;
    int dpd_mode = 0;
    char cmd[BUFLEN_128] = {0};
    char buf[BUFLEN_2] = {0};

    snprintf(cmd, sizeof(cmd), "%s dpden",
             ECOMODE_SCRIPT_FILE);
    if ((fp = popen(cmd, "r")) != NULL)
    {
        if (fgets(buf, sizeof(buf), fp) != NULL)
        {
            dpd_mode = atoi(buf);
        }
        pclose(fp);
    }

    wifi_hal_dbg_print("%s:%d DPD Feature is %s!!! \n", __func__, __LINE__, (dpd_mode ? "enabled" : "disabled"));
    return dpd_mode;
}

/**
 * @brief API to check EDPD control enabled in CPE.
 *
 * @return int - Return 1 if feature enabled else returns 0.
 */
static int check_edpdctl_enabled()
{
    FILE *fp = NULL;
    int edpd_status = 0;
    char cmd[BUFLEN_128] = {0};
    char buf[BUFLEN_2] = {0};

    snprintf(cmd, sizeof(cmd), "nvram kget wl_edpdctl_enable");
    if ((fp = popen(cmd, "r")) != NULL)
    {
        if (fgets(buf, sizeof(buf), fp) != NULL)
        {
            edpd_status = atoi(buf);
        }
        pclose(fp);
    }

    wifi_hal_dbg_print("%s:%d EDPD Power control is %s!!! \n", __func__, __LINE__, (edpd_status ? "enabled" : "disabled"));

    return edpd_status;
}

/**
 * @brief API to export GPIO Pin.
 *
 * @param pin - GPIO pin number
 * @return int - RETURN_OK upon successful, RETURN_ERR upon error
 */
static int export_gpio(const int pin)
{
    int fd = open(GPIO_EXPORT_PATH, O_WRONLY);
    if (fd < 0)
    {
        wifi_hal_error_print("%s:%d  Unable to open GPIO export file", __func__, __LINE__);
        return RETURN_ERR;
    }
    char buffer[BUFLEN_128] = {0};
    int len = snprintf(buffer, sizeof(buffer), "%d", pin);
    if (write(fd, buffer, len) != len)
    {
        wifi_hal_error_print("%s:%d  Unable to export GPIO%d!!! \n", __func__, __LINE__, pin);
        close(fd);
        return RETURN_ERR;
    }
    close(fd);

    wifi_hal_dbg_print("%s:%d Exported GPIO %d!!!\n", __func__, __LINE__, pin);
    return RETURN_OK;
}

/**
 * @brief API to unexport GPIO Pin.
 *
 * @param pin - GPIO pin number
 * @return int - 0 upon successful, -1 upon error
 */
static int unexport_gpio(const int pin)
{
    int fd = open(GPIO_UNEXPORT_PATH, O_WRONLY);
    if (fd < 0)
    {
        wifi_hal_error_print("%s:%d  Unable to open GPIO unexport file \n", __func__, __LINE__);
        return RETURN_ERR;
    }
    char buffer[BUFLEN_128] = {0};
    int len = snprintf(buffer, sizeof(buffer), "%d", pin);
    if (write(fd, buffer, len) != len)
    {
        wifi_hal_error_print("%s:%d  Unable to unexport GPIO%d!!! \n", __func__, __LINE__,pin);
        close(fd);
        return RETURN_ERR;
    }
    close(fd);
    wifi_hal_dbg_print("%s:%d  Unexported GPIO %d!!!\n", __func__, __LINE__, pin);

    return RETURN_OK;
}
/**
 * @brief API to set GPIO Pin direction.
 *
 * @param pin - GPIO pin number
 * @param direction - GPIO direction either "out" or "in"
 * @return int - RETURN_OK upon successful, RETURN_ERR upon error
 */
static int set_gpio_direction(const int pin, const char *direction)
{
    char path[BUFLEN_128] = {0};
    snprintf(path, sizeof(path), GPIO_DIRECTION_PATH, pin);
    int fd = open(path, O_WRONLY);
    if (fd < 0)
    {
        perror("Unable to open GPIO direction file");
        return RETURN_ERR;
    }
    if (write(fd, direction, strlen(direction)) != (int)strlen(direction))
    {
        wifi_hal_error_print("%s:%d Unable to set GPIO direction \n", __func__, __LINE__);
        close(fd);
        return RETURN_ERR;
    }
    close(fd);
    wifi_hal_dbg_print("%s:%d Set GPIO %d direction to %s. \n", __func__, __LINE__, pin, direction);

    return RETURN_OK;
}

/**
 * @brief API to write value to gpio pin
 *
 * @param pin - GPIO pin number
 * @param value - value could be either 1 or 0
 * @return int - RETURN_OK upon successful, RETURN_ERR upon error
 */
static int write_gpio_value(int pin, int value)
{
    char path[BUFLEN_128] = {0};
    snprintf(path, sizeof(path), GPIO_VALUE_PATH, pin);
    int fd = open(path, O_WRONLY);
    if (fd < 0)
    {
        perror("Unable to open GPIO value file");
        return RETURN_ERR;
    }
    if (write(fd, value ? "1" : "0", 1) != 1)
    {
        wifi_hal_error_print("%s:%d Unable to write GPIO value \n", __func__, __LINE__);
        close(fd);
        return RETURN_ERR;
    }
    close(fd);
    wifi_hal_dbg_print("%s:%d Write value %d on GPIO %d \n", __func__, __LINE__, value, pin);
    return RETURN_OK;
}

/**
 * @brief Set the gpio configuration for eco mode
 *
 * @description Once we put the board in eco mode, we must need to disconnect
 * power from soc chip from wlan chip. Its using change GPIO configuration.
 * @param wl_idx  - Radio index
 * @param eco_pwr_down - Indicate power down or up radio
 * @return int - 0 on success , -1 on error
 */
int platform_set_gpio_config_for_ecomode(const int wl_idx, const bool eco_pwr_down)
{
    if (!check_edpdctl_enabled() && !check_dpd_feature_enabled())
    {
        wifi_hal_error_print("%s:%d  EDPD Feature control configuration NOT enabled\n", __func__, __LINE__);
        return -1;
    }

    int gpio_pin = (wl_idx == 0) ? GPIO_PIN_24G_RADIO : GPIO_PIN_5G_RADIO;
    int value = (eco_pwr_down) ? 1 : 0;
    int rc = 0;

    rc = export_gpio(gpio_pin);
    if (rc != RETURN_OK)
    {
        wifi_hal_error_print("%s:%d Failed to export gpio %d \n", __func__, __LINE__, gpio_pin);
        goto EXIT;
    }

    rc = set_gpio_direction(gpio_pin, GPIO_DIRECTION_OUT);
    if (rc != RETURN_OK)
    {
        wifi_hal_dbg_print("%s:%d Failed to set direction for gpio %d \n", __func__, __LINE__, gpio_pin);
        goto EXIT;
    }

    rc = write_gpio_value(gpio_pin, value);
    if (rc != RETURN_OK)
    {
        wifi_hal_error_print("%s:%d Failed to set value for gpio %d \n", __func__, __LINE__, gpio_pin);
        goto EXIT;
    }

    unexport_gpio(gpio_pin);

    wifi_hal_dbg_print("%s:%d For wl%d, configured the gpio to %s the PCIe interface \n", __func__, __LINE__, wl_idx, (eco_pwr_down ? "power down" : "power up"));
EXIT:
    return rc;
}

/**
 * @brief Set the ecomode for radio object
 *
 * @description To make enable or disable eco mode, we are using broadcom
 * single control wifi.sh script.
 * @param wl_idx  - Radio index
 * @param eco_pwr_down - Indicate power down or up radio
 * @return int - 0 on success , -1 on error
 */
int platform_set_ecomode_for_radio(const int wl_idx, const bool eco_pwr_down)
{
    if (!check_edpdctl_enabled() && !check_dpd_feature_enabled())
    {
        wifi_hal_error_print("%s:%d  EDPD Feature control configuration NOT enabled\n", __func__, __LINE__);
        return -1;
    }

    char cmd[BUFLEN_128] = {0};
    int rc = 0;

    /* Put radio into eco mode (power down) */
    if (eco_pwr_down)
        snprintf(cmd, sizeof(cmd), "sh %s edpddn wl%d",
                 ECOMODE_SCRIPT_FILE, wl_idx);
    else
        snprintf(cmd, sizeof(cmd), "sh %s edpdup wl%d",
                 ECOMODE_SCRIPT_FILE, wl_idx);

    rc = system(cmd);
    if (rc == 0)
    {
        wifi_hal_dbg_print("%s:%d cmd [%s] successful \n", __func__, __LINE__, cmd);
    }
    else
    {
        wifi_hal_error_print("%s:%d cmd [%s] unsuccessful \n", __func__, __LINE__, cmd);
    }

    return rc;
}
#endif // defined (ENABLED_EDPD) && defined(_SR213_PRODUCT_REQ_)

int platform_set_txpower(void* priv, uint txpower)
{
    return 0;
}

int platform_set_neighbor_report(uint index, uint add, mac_address_t mac)
{
    wifi_hal_info_print("%s:%d Enter %d\n", __func__, __LINE__,index);
    wifi_NeighborReport_t nbr_report;
    memcpy(nbr_report.bssid,mac,sizeof(mac_address_t));
    wifi_setNeighborReports(index,add, &nbr_report);

    return 0;
}
#if defined (_SR213_PRODUCT_REQ_)
#define SKY_VENDOR_OUI "DD0480721502"
int platform_get_vendor_oui(char *vendor_oui, int vendor_oui_len)
{
    if (NULL == vendor_oui) {
        wifi_hal_error_print("%s:%d  Invalid parameter \n", __func__, __LINE__);
        return -1;
    }
    strncpy(vendor_oui, SKY_VENDOR_OUI, vendor_oui_len - 1);

    return 0;
}
#else
int platform_get_vendor_oui(char *vendor_oui, int vendor_oui_len)
{
    return -1;
}
#endif /*_SR213_PRODUCT_REQ_ */

#if defined(TCXB7_PORT) || defined(TCXB8_PORT) || defined(XB10_PORT)

typedef struct sta_list {
    mac_address_t *macs;
    unsigned int num;
} sta_list_t;

static int get_sta_list_handler(struct nl_msg *msg, void *arg)
{
    int rem_mac, i;
    struct nlattr *nlattr;
    struct nlattr *tb[NL80211_ATTR_MAX + 1];
    struct nlattr *tb_vendor[RDK_VENDOR_ATTR_MAX + 1];
    struct genlmsghdr *gnlh = nlmsg_data(nlmsg_hdr(msg));
    static struct nla_policy sta_policy[RDK_VENDOR_ATTR_MAX + 1] = {
        [RDK_VENDOR_ATTR_MAC] = { .type = NLA_BINARY },
        [RDK_VENDOR_ATTR_STATION_NUM] = { .type = NLA_U32 },
        [RDK_VENDOR_ATTR_STATION_LIST] = { .type = NLA_NESTED },
    };
    sta_list_t *sta_list = arg;

    if (nla_parse(tb, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0), genlmsg_attrlen(gnlh, 0),
        NULL) < 0) {
        wifi_hal_error_print("%s:%d Failed to parse vendor data\n", __func__, __LINE__);
        return NL_SKIP;
    }

    if (tb[NL80211_ATTR_VENDOR_DATA] == NULL) {
        wifi_hal_error_print("%s:%d Vendor data is missing\n", __func__, __LINE__);
        return NL_SKIP;
    }

    nlattr = tb[NL80211_ATTR_VENDOR_DATA];
    if (nla_parse(tb_vendor, RDK_VENDOR_ATTR_MAX, nla_data(nlattr), nla_len(nlattr),
        sta_policy) < 0) {
        wifi_hal_error_print("%s:%d Failed to parse vendor attribute\n", __func__, __LINE__);
        return NL_SKIP;
    }

    if (tb_vendor[RDK_VENDOR_ATTR_STATION_NUM] == NULL) {
        wifi_hal_error_print("%s:%d STA number data is missing\n", __func__, __LINE__);
        return NL_SKIP;
    }

    sta_list->num = nla_get_u32(tb_vendor[RDK_VENDOR_ATTR_STATION_NUM]);
    if (sta_list->num == 0) {
        sta_list->macs = NULL;
        return NL_SKIP;
    }

    sta_list->macs = calloc(sta_list->num, sizeof(mac_address_t));

    if (tb_vendor[RDK_VENDOR_ATTR_STATION_LIST] == NULL) {
        wifi_hal_error_print("%s:%d STA list data is missing\n", __func__, __LINE__);
        goto error;
    }

    i = 0;
    nla_for_each_nested(nlattr, tb_vendor[RDK_VENDOR_ATTR_STATION_LIST], rem_mac) {
        if (i >= sta_list->num) {
            wifi_hal_error_print("%s:%d STA list overflow\n", __func__, __LINE__);
            goto error;
        }

        if (nla_len(nlattr) != sizeof(mac_address_t)) {
            wifi_hal_error_print("%s:%d Wrong MAC address len\n", __func__, __LINE__);
            goto error;
        }

        memcpy(sta_list->macs[i], nla_data(nlattr), sizeof(mac_address_t));

        i++;
    }

    if (i != sta_list->num) {
        wifi_hal_error_print("%s:%d Failed to receive all stations\n", __func__, __LINE__);
        goto error;
    }

    return NL_SKIP;

error:
    free(sta_list->macs);
    sta_list->macs = NULL;
    sta_list->num = 0;
    return NL_SKIP;
}

static int get_sta_list(wifi_interface_info_t *interface, sta_list_t *sta_list)
{
    int ret;
    struct nl_msg *msg;

    msg = nl80211_drv_vendor_cmd_msg(g_wifi_hal.nl80211_id, interface, 0, OUI_COMCAST,
        RDK_VENDOR_NL80211_SUBCMD_GET_STATION_LIST);
    if (msg == NULL) {
        wifi_hal_error_print("%s:%d Failed to create NL command\n", __func__, __LINE__);
        return RETURN_ERR;
    }

    ret = nl80211_send_and_recv(msg, get_sta_list_handler, sta_list, NULL, NULL);
    if (ret) {
        wifi_hal_error_print("%s:%d Failed to send NL message\n", __func__, __LINE__);
        return RETURN_ERR;
    }

    return RETURN_OK;
}

static int standard_to_str(uint32_t standard, char *buf, size_t buf_size)
{
    char *std_str;

    switch (standard) {
        case RDK_VENDOR_NL80211_STANDARD_A: std_str = "a"; break;
        case RDK_VENDOR_NL80211_STANDARD_B: std_str = "b"; break;
        case RDK_VENDOR_NL80211_STANDARD_G: std_str = "g"; break;
        case RDK_VENDOR_NL80211_STANDARD_N: std_str = "n"; break;
        case RDK_VENDOR_NL80211_STANDARD_AC: std_str = "ac"; break;
        case RDK_VENDOR_NL80211_STANDARD_AD: std_str = "ad"; break;
        case RDK_VENDOR_NL80211_STANDARD_AX: std_str = "ax"; break;
        case RDK_VENDOR_NL80211_STANDARD_BE: std_str = "be"; break;
        default: std_str = ""; break;
    }

    strncpy(buf, std_str, buf_size - 1);

    return 0;
}

static int bw_to_str(uint8_t bw, char *buf, size_t buf_size)
{
    char *bw_str;

    switch (bw) {
        case RDK_VENDOR_NL80211_CHAN_WIDTH_20: bw_str = "20"; break;
        case RDK_VENDOR_NL80211_CHAN_WIDTH_40: bw_str = "40"; break;
        case RDK_VENDOR_NL80211_CHAN_WIDTH_80: bw_str = "80"; break;
        case RDK_VENDOR_NL80211_CHAN_WIDTH_160: bw_str = "160"; break;
        case RDK_VENDOR_NL80211_CHAN_WIDTH_320: bw_str = "320"; break;
        default: bw_str = ""; break;
    }

    strncpy(buf, bw_str, buf_size - 1);

    return 0;
}

static int get_sta_stats_handler(struct nl_msg *msg, void *arg)
{
    int i;
    struct nlattr *nlattr;
    struct nlattr *tb[NL80211_ATTR_MAX + 1];
    struct nlattr *tb_vendor[RDK_VENDOR_ATTR_MAX + 1];
    struct nl80211_sta_flag_update *sta_flags;
    struct genlmsghdr *gnlh = nlmsg_data(nlmsg_hdr(msg));
    static struct nla_policy sta_policy[RDK_VENDOR_ATTR_MAX + 1] = {
        [RDK_VENDOR_ATTR_MAC] = { .type = NLA_BINARY, .minlen = ETHER_ADDR_LEN },
        [RDK_VENDOR_ATTR_STA_INFO_STA_FLAGS] = { .type = NLA_BINARY,
            .minlen = sizeof(struct nl80211_sta_flag_update) },
        [RDK_VENDOR_ATTR_STA_INFO_RX_BITRATE_LAST] = { .type = NLA_U32 },
        [RDK_VENDOR_ATTR_STA_INFO_TX_BITRATE_LAST] = { .type = NLA_U32 },
        [RDK_VENDOR_ATTR_STA_INFO_SIGNAL_AVG] = { .type = NLA_S32 },
        [RDK_VENDOR_ATTR_STA_INFO_TX_RETRIES_PERCENT] = { .type = NLA_U32 },
        [RDK_VENDOR_ATTR_STA_INFO_ACTIVE] = { .type = NLA_U8 },
        [RDK_VENDOR_ATTR_STA_INFO_OPER_STANDARD] = { .type = NLA_U32 },
        [RDK_VENDOR_ATTR_STA_INFO_OPER_CHANNEL_BW] = { .type = NLA_U8 },
        [RDK_VENDOR_ATTR_STA_INFO_SNR] = { .type = NLA_S32 },
        [RDK_VENDOR_ATTR_STA_INFO_TX_PACKETS_ACK] = { .type = NLA_U64 },
        [RDK_VENDOR_ATTR_STA_INFO_TX_PACKETS_NO_ACK] = { .type = NLA_U64 },
        [RDK_VENDOR_ATTR_STA_INFO_TX_BYTES64] = { .type = NLA_U64 },
        [RDK_VENDOR_ATTR_STA_INFO_RX_BYTES64] = { .type = NLA_U64 },
        [RDK_VENDOR_ATTR_STA_INFO_SIGNAL_MIN] = { .type = NLA_S32 },
        [RDK_VENDOR_ATTR_STA_INFO_SIGNAL_MAX] = { .type = NLA_S32 },
        [RDK_VENDOR_ATTR_STA_INFO_ASSOC_NUM] = { .type = NLA_U64 },
        [RDK_VENDOR_ATTR_STA_INFO_TX_PACKETS64] = { .type = NLA_U64 },
        [RDK_VENDOR_ATTR_STA_INFO_RX_PACKETS64] = { .type = NLA_U64 },
        [RDK_VENDOR_ATTR_STA_INFO_TX_ERRORS] = { .type = NLA_U64 },
        [RDK_VENDOR_ATTR_STA_INFO_TX_RETRANSMIT] = { .type = NLA_U64 },
        [RDK_VENDOR_ATTR_STA_INFO_TX_FAILED_RETRIES] = { .type = NLA_U64 },
        [RDK_VENDOR_ATTR_STA_INFO_TX_RETRIES] = { .type = NLA_U64 },
        [RDK_VENDOR_ATTR_STA_INFO_TX_MULT_RETRIES] = { .type = NLA_U64 },
        [RDK_VENDOR_ATTR_STA_INFO_TX_RATE_MAX] = { .type = NLA_U32 },
        [RDK_VENDOR_ATTR_STA_INFO_RX_RATE_MAX] = { .type = NLA_U32 },
        [RDK_VENDOR_ATTR_STA_INFO_SPATIAL_STREAM_NUM] = { .type = NLA_U8 },
        [RDK_VENDOR_ATTR_STA_INFO_TX_FRAMES] = {.type = NLA_U64 },
        [RDK_VENDOR_ATTR_STA_INFO_RX_RETRIES] = { .type = NLA_U64 },
        [RDK_VENDOR_ATTR_STA_INFO_RX_ERRORS] = {. type = NLA_U64 },
    };
    wifi_associated_dev3_t *stats = arg;

    if (nla_parse(tb, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0), genlmsg_attrlen(gnlh, 0),
        NULL) < 0) {
        wifi_hal_error_print("%s:%d Failed to parse vendor data\n", __func__, __LINE__);
        return NL_SKIP;
    }

    if (tb[NL80211_ATTR_VENDOR_DATA] == NULL) {
        wifi_hal_error_print("%s:%d Vendor data is missing\n", __func__, __LINE__);
        return NL_SKIP;
    }

    nlattr = tb[NL80211_ATTR_VENDOR_DATA];
    if (nla_parse(tb_vendor, RDK_VENDOR_ATTR_MAX, nla_data(nlattr), nla_len(nlattr),
        sta_policy) < 0) {
        wifi_hal_error_print("%s:%d Failed to parse vendor attribute\n", __func__, __LINE__);
        return NL_SKIP;
    }

    for (i = 0; i <= RDK_VENDOR_ATTR_MAX; i++) {
        if (sta_policy[i].type != 0 && tb_vendor[i] == NULL) {
            wifi_hal_error_print("%s:%d STA info %d attribute is missing\n", __func__,
                __LINE__, i);
            return NL_SKIP;
        }
    }

    memcpy(stats->cli_MACAddress, nla_data(tb_vendor[RDK_VENDOR_ATTR_MAC]),
        nla_len(tb_vendor[RDK_VENDOR_ATTR_MAC]));
    sta_flags = nla_data(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_STA_FLAGS]);
    stats->cli_AuthenticationState = sta_flags->mask & (1 << NL80211_STA_FLAG_AUTHORIZED) &&
        sta_flags->set & (1 << NL80211_STA_FLAG_AUTHORIZED);
    stats->cli_LastDataUplinkRate =
        nla_get_u32(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_RX_BITRATE_LAST]);
    stats->cli_LastDataDownlinkRate =
        nla_get_u32(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_TX_BITRATE_LAST]);
    stats->cli_RSSI = nla_get_s32(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_SIGNAL_AVG]);
    stats->cli_SignalStrength = stats->cli_RSSI;
    stats->cli_MinRSSI = nla_get_s32(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_SIGNAL_MIN]);
    stats->cli_MaxRSSI = nla_get_s32(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_SIGNAL_MAX]);
    stats->cli_Retransmissions =
        nla_get_u32(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_TX_RETRIES_PERCENT]);
    stats->cli_Active = nla_get_u8(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_ACTIVE]);
    standard_to_str(nla_get_u32(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_OPER_STANDARD]),
        stats->cli_OperatingStandard, sizeof(stats->cli_OperatingStandard));
    bw_to_str(nla_get_u8(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_OPER_CHANNEL_BW]),
        stats->cli_OperatingChannelBandwidth, sizeof(stats->cli_OperatingChannelBandwidth));
    stats->cli_SNR = nla_get_s32(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_SNR]);
    stats->cli_DataFramesSentAck =
        nla_get_u64(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_TX_PACKETS_ACK]);
    stats->cli_DataFramesSentNoAck =
        nla_get_u64(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_TX_PACKETS_NO_ACK]);
    stats->cli_BytesSent = nla_get_u64(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_TX_BYTES64]);
    stats->cli_BytesReceived = nla_get_u64(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_RX_BYTES64]);
    stats->cli_AuthenticationFailures =
        nla_get_u32(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_AUTH_FAILS]);
    stats->cli_Associations = nla_get_u64(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_ASSOC_NUM]);
    stats->cli_PacketsSent = nla_get_u64(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_TX_PACKETS64]);
    stats->cli_PacketsReceived =
        nla_get_u64(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_RX_PACKETS64]);
    stats->cli_ErrorsSent =
        nla_get_u64(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_TX_ERRORS]);
    stats->cli_RetransCount =
        nla_get_u64(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_TX_RETRANSMIT]);
    stats->cli_FailedRetransCount =
        nla_get_u64(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_TX_FAILED_RETRIES]);
    stats->cli_RetryCount =    nla_get_u64(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_TX_RETRIES]);
    stats->cli_MultipleRetryCount =
        nla_get_u64(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_TX_MULT_RETRIES]);
    stats->cli_MaxDownlinkRate =
        nla_get_u32(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_TX_RATE_MAX]);
    stats->cli_MaxUplinkRate =
        nla_get_u32(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_RX_RATE_MAX]);
    stats->cli_activeNumSpatialStreams =
        nla_get_u8(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_SPATIAL_STREAM_NUM]);
    stats->cli_TxFrames = nla_get_u64(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_TX_FRAMES]);
    stats->cli_RxRetries = nla_get_u64(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_RX_RETRIES]);
    stats->cli_RxErrors = nla_get_u64(tb_vendor[RDK_VENDOR_ATTR_STA_INFO_RX_ERRORS]);

    /* Assume the default packet size for wifi blaster is 1470 */
    if ((stats->cli_BytesSent / 1470) >= stats->cli_PacketsSent) {
        stats->cli_DataFramesSentAck = (stats->cli_BytesSent / 1470) -
            stats->cli_DataFramesSentNoAck;
    }

    return NL_SKIP;
}

static int get_sta_stats(wifi_interface_info_t *interface, mac_address_t mac,
    wifi_associated_dev3_t *stats)
{
    struct nl_msg *msg;
    struct nlattr *nlattr;
    int ret = RETURN_ERR;

    msg = nl80211_drv_vendor_cmd_msg(g_wifi_hal.nl80211_id, interface, 0, OUI_COMCAST,
        RDK_VENDOR_NL80211_SUBCMD_GET_STATION);
    if (msg == NULL) {
        wifi_hal_error_print("%s:%d Failed to create NL command\n", __func__, __LINE__);
        return RETURN_ERR;
    }

    nlattr = nla_nest_start(msg, NL80211_ATTR_VENDOR_DATA);
    if (nla_put(msg, RDK_VENDOR_ATTR_MAC, ETHER_ADDR_LEN, mac) < 0) {
        wifi_hal_error_print("%s:%d Failed to put mac address\n", __func__, __LINE__);
        nlmsg_free(msg);
        return RETURN_ERR;
    }
    nla_nest_end(msg, nlattr);

    ret = nl80211_send_and_recv(msg, get_sta_stats_handler, stats, NULL, NULL);
    if (ret) {
        wifi_hal_error_print("%s:%d Failed to send NL message\n", __func__, __LINE__);
        return RETURN_ERR;
    }

    return ret;
}

INT wifi_getApAssociatedDeviceDiagnosticResult3(INT apIndex,
    wifi_associated_dev3_t **associated_dev_array, UINT *output_array_size)
{
    int ret;
    unsigned int i;
    sta_list_t sta_list = {};
    wifi_interface_info_t *interface;

    interface = get_interface_by_vap_index(apIndex);
    if (interface == NULL) {
        wifi_hal_error_print("%s:%d Failed to get interface for index %d\n", __func__, __LINE__,
            apIndex);
        return RETURN_ERR;
    }

    ret = get_sta_list(interface, &sta_list);
    if (ret != RETURN_OK) {
        wifi_hal_error_print("%s:%d Failed to get sta list\n", __func__, __LINE__);
        goto exit;
    }

    *associated_dev_array = sta_list.num ?
        calloc(sta_list.num, sizeof(wifi_associated_dev3_t)) : NULL;
    *output_array_size = sta_list.num;

    for (i = 0; i < sta_list.num; i++) {
        ret = get_sta_stats(interface, sta_list.macs[i], &(*associated_dev_array)[i]);
        if (ret != RETURN_OK) {
            wifi_hal_error_print("%s:%d Failed to get sta stats\n", __func__, __LINE__);
            free(*associated_dev_array);
            *associated_dev_array = NULL;
            *output_array_size = 0;
            goto exit;
        }
    }

exit:
    free(sta_list.macs);
    return ret;
}

#endif // TCXB7_PORT || TCXB8_PORT || XB10_PORT

