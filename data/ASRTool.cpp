#include "ASRTool.h"
#include "ASRHandler.h"
#include <QDebug>


extern void DoRecognize(FreetalkStream *session, MYCONTEXT *ctx);
// 获取临时目录路径
static const char *temp_folder() {
    // TODO:
    // TMPDIR 为 unix 平台临时目录环境变量
    const char *p = getenv("TMPDIR");
    if (!p) p = getenv("TEMP");  // TEMP 为 windows 平台临时目录环境变量
    if (!p) p = getenv("TMP");   // TMP 为 windows 平台临时目录环境变量
    if (!p) p = "/tmp";  // 未能通过环境变量获取临时目录时使用默认值
    return p;
}

static int cancelled_count = 0;
static int aborted_count = 0;
static int success_count = 0;
static int failure_count = 0;

// 统计
void DoCount(HciError code) {
    if (code == HciError::OK)
        ++success_count;
    else if (code == HciError::CANCELLED)
        ++cancelled_count;
    else if (code == HciError::ABORTED)
        ++aborted_count;
    else
        ++failure_count;
}

ASRTool::ASRTool(QObject *parent) : QObject(parent)
{
}

ASRTool::~ASRTool()
{
    exit();
}

bool ASRTool::init()
{
    qDebug() << "ASR功能初始化。";
    HciSdkConfig config;

    // 创建 SDK 对象
    m_sdk = HciSdk::New();
    wp = m_sdk;  // 在程序退出时确认引用计数为零
    qDebug() << "创建SDK对象。";

    // 日志级别设置为 T (输出所有日志级别的内容)
    m_sdk->set_log_level(hci::sdk::LogLevel::T);

    // 设置 SDK 初始化参数
    config.set_appkey("aicp_app");
    config.set_secret("QWxhZGRpbjpvcGVuIHNlc2FtZQ");
    config.set_sys_url("https://172.16.100.180:22801/");
    config.set_cap_url("http://172.16.100.180:22800/");
    config.set_data_path(temp_folder());
    config.set_verify_ssl(false);

    qDebug() << "sys url" << config.sys_url();
    qDebug() << "cap url" << config.cap_url();
    qDebug() << "hci.log路径：" << config.data_path();

    qDebug() << "SDK初始化参数设置。";
    auto code = m_sdk->Init(&config);
    if (code != HciError::OK) {
        qDebug() << "SDK初始化失败。";
        m_sdk.reset();
        return false;
    }

    qDebug() << "创建云端 FreetalkStream 会话";
    CloudAsrConfig asr_config;
    m_ftstm = FreetalkStream::NewCloud(m_sdk, &asr_config);
//    m_ftstm = FreetalkShortAudio::NewCloud(m_sdk, &asr_config);


    qDebug() << "云端 FreetalkStream 会话  --1 ";
//    assert(m_ftstm->state() == SessionState::Idle);
    qDebug() << "云端 FreetalkStream 会话  --2 ";

    return true;
}

void ASRTool::run(int len, char **data)
{
    // SDK 对象被关闭后，调用 this->OnSdkClosed()
    m_sdk->WaitForClosed(&ASRTool::onSdkClosed, this);

//    MYCONTEXT::alloc(m_ftstm.get(), m_sdk.get());
//    auto *ctx = MYCONTEXT::get(m_ftstm.get());

//    // TODO:字符串复制
//    ctx->len = len;
//    ctx->data = data;

//    ctx->seq = 0;
//    // 回调参数可以接收 HciTimer * 即可
//    ctx->timer = m_sdk->NewTimer([this, ctx](void *) {
//        m_ftstm->Stop((ctx->seq++ & 1)); });

//    // 首次触发在 2000ms 后，然后每隔 5000ms 触发一次
//    ctx->timer->Start(2000, 5000);

//    DoRecognize(m_ftstm.get(), ctx);

//    m_sdk->WaitForClosed([ctx]() {
//        // sdk 关闭后打印识别的统计结果
//        qDebug() << "success_count = %d\n" << success_count;
//        qDebug() << "failure_count = %d\n" << failure_count;
//        qDebug() << "aborted_count = %d\n" << aborted_count;
//        qDebug() << "canncelled_count = %d\n" << cancelled_count;
//        qDebug() << "remain = %d\n" << ctx->len;
//    });

}

void ASRTool::exit()
{
    if (!m_sdk) return;

    // 临时保存 sdk 对象引用
    auto sdk = m_sdk;

    // 关闭 SDK 对象，异步调用
    sdk->Close();

    auto HCI_UNUSED state = sdk->sdk_state();
    assert(state == SdkState::Closing || state == SdkState::Closed);

    // 同步等待 SDK 对象完成关闭
    sdk->WaitForClosed();

    state = sdk->sdk_state();
    assert(state == SdkState::Closed);

    m_sdk.reset();
}

void ASRTool::onSdkClosed()
{
    // sdk_ 可能已经在之前的回调中被置空，所以不要使用 sdk_
    m_sdk.reset();
}
