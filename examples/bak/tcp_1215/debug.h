#ifndef DEBUG_COMMON_H
#define DEBUG_COMMON_H

#define DRV_DBG_1 printk("[%s] 11111111111111111111111 \n", __func__);
#define DRV_DBG_2 printk("[%s] 22222222222222222222222 \n", __func__);
#define DRV_DBG_3 printk("[%s] 33333333333333333333333 \n", __func__);
#define DRV_DBG_4 printk("[%s] 44444444444444444444444 \n", __func__);
#define DRV_DBG_5 printk("[%s] 55555555555555555555555 \n", __func__);

#define DRV_D(...)		printk("[%s %d] ", __func__, __LINE__); printk(__VA_ARGS__); printk("\n")
#define DRV_DD(...)		printk("[%s %d] ", __func__, __LINE__); printk(__VA_ARGS__); printk("\n")
#define DRV_DDD(...)	printk("[%s %d] ", __func__, __LINE__); printk(__VA_ARGS__); printk("\n")
#define DRV_I(...)		printk("[%s %d][Info] ", __func__, __LINE__); printk(__VA_ARGS__); printk("\n")
#define DRV_W(...)		printk("[%s %d][Warn] ", __func__, __LINE__); printk(__VA_ARGS__); printk("\n")
#define DRV_E(...)		printk("[%s %d][Error] ", __func__, __LINE__);printk(__VA_ARGS__); printk("\n")


#define APP_DBG_1 printf("[%s] 11111111111111111111111 \n", __func__);
#define APP_DBG_2 printf("[%s] 22222222222222222222222 \n", __func__);
#define APP_DBG_3 printf("[%s] 33333333333333333333333 \n", __func__);
#define APP_DBG_4 printf("[%s] 44444444444444444444444 \n", __func__);
#define APP_DBG_5 printf("[%s] 55555555555555555555555 \n", __func__);

#define APPLOG_STD_I(...)	printf("[Info] "); printf(__VA_ARGS__); printf("\n")
#define APPLOG_STD_W(...)	printf("[Warn] "); printf(__VA_ARGS__); printf("\n")
#define APPLOG_STD_E(...)	printf("[Error] "); printf(__VA_ARGS__); printf("\n")

#define APPLOG_D(...)		printf("[%s %d] ", __func__, __LINE__); printf(__VA_ARGS__); printf("\n")
#define APPLOG_DD(...)		printf("[%s %d] ", __func__, __LINE__); printf(__VA_ARGS__); printf("\n")
#define APPLOG_DDD(...)		printf("[%s %d] ", __func__, __LINE__); printf(__VA_ARGS__); printf("\n")
#define APPLOG_I(...)		printf("[%s %d][Info] ", __func__, __LINE__); printf(__VA_ARGS__); printf("\n")
#define APPLOG_W(...)		printf("[%s %d][Warn] ", __func__, __LINE__); printf(__VA_ARGS__); printf("\n")
#define APPLOG_E(...)		printf("[%s %d][Error] ", __func__, __LINE__);printf(__VA_ARGS__); printf("\n")
#define APPLOG_TBD(...)		printf("[%s %d][TBD] ", __func__, __LINE__);printf(__VA_ARGS__); printf("\n")
#endif
