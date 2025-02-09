package com.example.sample1;

import java.io.InputStream;
import java.util.ArrayList;

import android.content.Context;
import android.content.res.AssetManager;

import com.elex.chatservice.model.ConfigManager;
import com.elex.chatservice.model.LanguageItem;
import com.elex.chatservice.model.LanguageManager;
import com.elex.chatservice.util.ConfigReader;

public class LanguageConfiger
{
	protected static void initLanguage(Context context)
	{
		ConfigManager.getInstance().gameLang = "en";

		 initFromINI(context);
//		initLiterally();
	}

	protected static void initFromINIInBackground(final Context context)
	{
		Runnable run = new Runnable()
		{
			@Override
			public void run()
			{
				initFromINI(context);
			}
		};

		Thread thread = new Thread(run);
		thread.start();
	}

	private static void initFromINI(final Context context)
	{
		try
		{
			AssetManager assetManager = context.getAssets();
			String filename = "local/text_zh_CN.ini";
			InputStream in = null;
			in = assetManager.open(filename);
			ConfigReader reader = new ConfigReader(in);
			ArrayList<LanguageItem> items = reader.getAllItems();
			LanguageManager.initChatLanguage(items);
		}
		catch (Exception e)
		{
		}
	}

	private static void initLiterally()
	{
		// key可以重复，textMap.put()时同key会覆盖
		LanguageItem[] chatLangArray = {
				// 内网包c++输出的
				new LanguageItem("E100068", "您未加入联盟，暂时无法使用联盟聊天频道"),
				new LanguageItem("115020", "加入"),
				new LanguageItem("105207", "禁言"),
				new LanguageItem("105209", "已禁言"),
				new LanguageItem("105210", "是否禁言：{0}？"),
				new LanguageItem("105300", "国家"),
				new LanguageItem("105302", "发送"),
				new LanguageItem("105304", "复制"),
				new LanguageItem("105307", "您发布的聊天消息过于频繁，请稍等！"),
				new LanguageItem("105308", "发送邮件"),
				new LanguageItem("105309", "查看领主"),
				new LanguageItem("105312", "屏蔽"),
				new LanguageItem("105313", "是否要屏蔽{0}，屏蔽后你将不会收到该领主的任何聊天信息和邮件，但是你可以在“设置-已屏蔽用户”解除对该领主的屏蔽。"),
				new LanguageItem("105315", "解除屏蔽"),
				new LanguageItem("105316", "聊天"),
				new LanguageItem("105321", "由{0}翻译"),
				new LanguageItem("105322", "原文"),
				new LanguageItem("105502", "下滑加载更多"),
				new LanguageItem("105602", "联盟"),
				new LanguageItem("108584", "邀请加入联盟"),
				new LanguageItem("115922", "屏蔽该领主留言"),
				new LanguageItem("115923", "屏蔽该联盟留言"),
				new LanguageItem("115925", "是否要屏蔽{0}，屏蔽后该领主将无法对您的联盟进行留言，但是你可以通过联盟管理来解除对该领主的屏蔽。"),
				new LanguageItem("115926", "是否要屏蔽{0}，屏蔽后该联盟将无法对您的联盟进行留言，但是你可以通过联盟管理来解除对该联盟的屏蔽。"),
				new LanguageItem("115929", "联盟留言"),
				new LanguageItem("115181", "系统"),
				new LanguageItem("115182", "我邀请了{0}加入我们的联盟，希望他能和我们一起并肩作战。"),
				new LanguageItem("115281", "查看战报"),
				new LanguageItem("115282", "抱歉，该战报已被删除，无法查看"),
				new LanguageItem("115168", "立即加入联盟获得金币"),
				new LanguageItem("115169", "{0}金币"),
				new LanguageItem("115170", "通过邮件发送"),
				new LanguageItem("105326", "翻译"),
				new LanguageItem("105327", "下拉可加载更多"),
				new LanguageItem("105328", "松开载入更多"),
				new LanguageItem("105324", "服务器即将在{0}分后停机更新"),
				new LanguageItem("105325", "服务器即将在{0}秒后停机更新"),
				new LanguageItem("115068", "立即加入"),
				new LanguageItem("confirm", "确定"),
				new LanguageItem("cancel_btn_label", "取消"),
				new LanguageItem("114110", "加载中..."),
				new LanguageItem("104932", "刷新"),
				new LanguageItem("105564", "全体联盟成员"),
				new LanguageItem("101205", "邮件"),
				new LanguageItem("105329", "论坛"),
				new LanguageItem("105522", "侦查报告"),
				new LanguageItem("105591", "小时"),
				new LanguageItem("105330", "是否重发此消息?"),
				new LanguageItem("105332", "发送王国公告将消耗1个 {0}！"),
				new LanguageItem("137450", "{0}集结的队伍挑战了大魔王"),
				new LanguageItem("105333", "领主大人，您的 {0} 不足，花费一些金币即可发送王国公告！"),
				new LanguageItem("104371", "喇叭"),
				new LanguageItem("104912", "领主大人，您的金币不足，赶快去购买一些吧！"),
				new LanguageItem("105331", "公告"),
				new LanguageItem("115100", "联盟阶级1"),
				new LanguageItem("115101", "联盟阶级2"),
				new LanguageItem("115102", "联盟阶级3"),
				new LanguageItem("115104", "联盟阶级5"),
				new LanguageItem("103001", "VIP{0}"),
				new LanguageItem("105348", "是否将{0}从群组中移除?"),
				new LanguageItem("105349", "是否将{0}加入群组?"),
				new LanguageItem("105350", "是否确认退出群组?"),
				new LanguageItem("105351", "是否将群组改名为{0}？"),
				new LanguageItem("105352", "{0}条新消息"),
				new LanguageItem("105353", "以下为新消息"),
				new LanguageItem("105354", "多人聊天"),
				new LanguageItem("105355", "创建者"),
				new LanguageItem("105344", "退出群组"),
				new LanguageItem("119004", "更换名称"),
				new LanguageItem("113907", "搜索"),
				new LanguageItem("105356", "已加入"),
				new LanguageItem("105357", "搜索结果"),
				new LanguageItem("105369", "以下是最近{0}条新消息"),
				new LanguageItem("103731", "逐鹿天下游戏工作室"),
				new LanguageItem("105569", "公告"),
				new LanguageItem("103738", "邀请函"),
				new LanguageItem("103783", "退盟通知"),
				new LanguageItem("133053", "黑骑士活动战报"),
				new LanguageItem("115337", "攻击联盟堡垒胜利"),
				new LanguageItem("115338", "攻击联盟堡垒失败"),
				new LanguageItem("115339", "防守联盟堡垒胜利 "),
				new LanguageItem("115340", "防守联盟堡垒失败"),
				new LanguageItem("132000", "联系MOD"),
				new LanguageItem("108675", "城堡遗迹探险报告"),
				new LanguageItem("105519", "战斗报告"),
				new LanguageItem("105590", "分钟"),
				new LanguageItem("105591", "小时"),
				new LanguageItem("105592", "天"),
				new LanguageItem("105593", "之前"),
				new LanguageItem("3000002", "阿加莎"),
				new LanguageItem("105734", "您的城堡已经被迫迁走！"),
				new LanguageItem("105735", "由于您长时间未登录，您的城堡被强制迁城到其他地点！"),
				new LanguageItem("138067", "远古战场:王国奖励"),
				new LanguageItem("138068", "尊敬的领主大人:" + "\n    远古战场已经关闭，有人壮志成城而来，满载荣誉而归；有人心怀理想踏入这片战场，最后遗憾而归。" + "\n    您的王国排名{0}"
						+ "\n本次活动王国前三名为:" + "\n    第一名:王国{1}" + "\n    第二名:王国{2}" + "\n    第三名:王国{3}"),
				new LanguageItem("105570", "您真的要删除该邮件吗？"),
				new LanguageItem("105512", "请领取奖励后再删除邮件！"),
				new LanguageItem("105599", "没有邮件可以删除"),
				new LanguageItem("108523", "删除"),
				new LanguageItem("4100013", "邮件已锁定，请解锁后再删除"),
				new LanguageItem("4100014", "邮件中存在未领奖励或者锁定的邮件，这些邮件已为您保留"),
				new LanguageItem("114010", "新手迁城"),
				new LanguageItem("101007", "来自{0}的礼物"),
				new LanguageItem("115295", "迁城邀请"),
				new LanguageItem("114012", "入盟申请"),
				new LanguageItem("115103", "联盟阶级4"),
				new LanguageItem("105718", "消灭敌军活动"),
				new LanguageItem("133017", "联盟积分奖励"),
				new LanguageItem("105550", "收治伤兵"),
				new LanguageItem("105710", "占领宫殿"),
				new LanguageItem("105711", "占领投石机"),
				new LanguageItem("105537", "占领成功！"),
				new LanguageItem("105712", "你成功占领了宫殿"),
				new LanguageItem("105713", "你成功占领了投石机"),
				new LanguageItem("105538", "您成功占领了{0}（X：{1} Y：{2}）"),
				new LanguageItem("105708", "侦查宫殿成功！"),
				new LanguageItem("105709", "侦查投石机成功！"),
				new LanguageItem("115356", "侦查联盟堡垒成功！"),
				new LanguageItem("105527", "侦查成功！"),
				new LanguageItem("115312", "联盟堡垒{0}"),
				new LanguageItem("114101", "已侦查到{0}的信息"),
				new LanguageItem("138039", "战场"),
				new LanguageItem("114121", "资源帮助报告"),
				new LanguageItem("114000", "绑定Google+成功"),
				new LanguageItem("114002", "绑定Facebook成功"),
				new LanguageItem("105385", "信箱里一封信都没有，下次再来看看吧"),
				new LanguageItem("111665", "查看属性"),
				new LanguageItem("105778", "是否要举报{0}的自定义头像？"),
				new LanguageItem("105781", "举报头像成功"),
				new LanguageItem("105782", "您已经举报过该领主的头像了"),
				new LanguageItem("137451", "挑战世界BOSS战报"),
				new LanguageItem("105516", "资源采集报告"),
				new LanguageItem("103758", "首杀奖励"),
				new LanguageItem("105373", "您确定要清空聊天室信息吗？"),
				new LanguageItem("105374", "您真的要删除这些邮件吗？"),
				new LanguageItem("105547", "{0}攻击{1}"),
				new LanguageItem("102187", "我的部队"),
				new LanguageItem("108678", "我的城堡"),
				new LanguageItem("105384", "信息"),
				new LanguageItem("115540", "攻击国旗胜利"),
				new LanguageItem("115541", "攻击国旗失败"),
				new LanguageItem("115542", "防守国旗胜利"),
				new LanguageItem("115543", "防守国旗失败"),
				new LanguageItem("115539", "侦查国旗成功！"),
				new LanguageItem("115534", "国旗"),
				new LanguageItem("111660", "我刚刚在铁匠铺在成功的锻造出了{0}"),
				new LanguageItem("105383", "活动"),
				new LanguageItem("105777", "举报头像"),
				new LanguageItem("105504", "内容"),
				new LanguageItem("105505", "收件人"),
				new LanguageItem("114102", "侦查失败"),
				new LanguageItem("114006", "野人消灭奖励"),
				new LanguageItem("114008", "扎营失败"),
				new LanguageItem("105523", "你的城市被侦查"),
				new LanguageItem("114014", "新的盟友"),
				new LanguageItem("114016", "新的入盟邀请"),
				new LanguageItem("114019", "设置您的名字和形象"),
				new LanguageItem("114020", "激活VIP"),
				new LanguageItem("114115", "积分目标奖励"),
				new LanguageItem("114116", "阶段排名奖励"),
				new LanguageItem("114117", "最强领主排名奖励"),
				new LanguageItem("114022", "关联账号有奖"),
				new LanguageItem("101227", "首充奖励"),
				new LanguageItem("114124", "预注册礼包"),
				new LanguageItem("114128", "盟主更换通知"),
				new LanguageItem("105068", "邀请大礼包"),
				new LanguageItem("105069", "邀请奖励"),
				new LanguageItem("113905", "绑定Game Center成功"),
				new LanguageItem("110014", "为了国王"),
				new LanguageItem("110100", "王位争夺战即将开启！"),
				new LanguageItem("110119", "征服者礼包"),
				new LanguageItem("110120", "捍卫者礼包"),
				new LanguageItem("110121", "支援者礼包"),
				new LanguageItem("105722", "成功购买月度好礼"),
				new LanguageItem("105732", "您的城堡已被摧毁！"),
				new LanguageItem("105742", "部队已返回"),
				new LanguageItem("111079", "火红宝箱"),
				new LanguageItem("111080", "星蓝宝箱"),
				new LanguageItem("111066", "残忍取消"),
				new LanguageItem("114135", "测试已结束"),
				new LanguageItem("133062", "个人积分排名奖励"),
				new LanguageItem("133026", "联盟积分排名奖励"),
				new LanguageItem("133100", "黑骑士即将攻城！"),
				new LanguageItem("137460", "大魔王消灭奖励"),
				new LanguageItem("137461", "集结失败"),
				new LanguageItem("133270", "黑骑士来袭活动开始"),
				new LanguageItem("114111", "加入联盟奖励"),
				new LanguageItem("114025", "绑定微博成功"),
				new LanguageItem("115464", "拒绝通知"),
				new LanguageItem("115476", "取出资源报告"),
				new LanguageItem("105714", "损失安慰"),
				new LanguageItem("105720", "城市被攻击"),
				new LanguageItem("105726", "青铜联盟礼物"),
				new LanguageItem("105727", "黑铁联盟礼物"),
				new LanguageItem("105728", "白银联盟礼物"),
				new LanguageItem("105729", "黄金联盟礼物"),
				new LanguageItem("105730", "传奇联盟礼物"),
				new LanguageItem("3110118", "战争守护提醒"),
				new LanguageItem("103786", "战斗无效"),
				new LanguageItem("105750", "Google Play 超值礼包"),
				new LanguageItem("101019", "诚挚邀请你参加我们的派对！"),
				new LanguageItem("110167", "国王万岁！"),
				new LanguageItem("110191", "官职任命"),
				new LanguageItem("115335", "{0}被摧毁了！"),
				new LanguageItem("114144", "“累积充值大回馈”活动奖励"),
				new LanguageItem("115399", "新的联盟堡垒解锁！"),
				new LanguageItem("105757", "队伍已返回！"),
				new LanguageItem("105759", "队伍已解散！"),
				new LanguageItem("138065", "远古战场即将开启"),
				new LanguageItem("115429", "联盟指令"),
				new LanguageItem("138099", "远古战场:个人奖励"),
				new LanguageItem("111504", "先贤的馈赠"),
				new LanguageItem("105567", "你的资源点被侦查"),
				new LanguageItem("114005", "对方使用了反侦察道具，无法获得侦查信息"),
				new LanguageItem("105524", "被{0}侦查"),
				new LanguageItem("137429", "{0}侦查了您的城堡"),
				new LanguageItem("137431", "{0}侦查了您的部队"),
				new LanguageItem("137430", "{0}侦查了您的资源田"),
				new LanguageItem("105118", "战斗失败"),
				new LanguageItem("105117", "战斗胜利"),
				new LanguageItem("111506", "成功驻扎{0}（X：{1} Y：{2}）并开始挖掘宝藏"),
				new LanguageItem("105700", "攻击宫殿成功"),
				new LanguageItem("105704", "攻击投石机成功"),
				new LanguageItem("105578", "攻城胜利"),
				new LanguageItem("105702", "宫殿防守成功"),
				new LanguageItem("105706", "投石机防守成功"),
				new LanguageItem("108554", "消灭敌军："),
				new LanguageItem("105579", "守城胜利"),
				new LanguageItem("105019", "部队损失数量"),
				new LanguageItem("115341", "消灭敌军:{0} 部队损失数量：{1}"),
				new LanguageItem("105701", "攻击宫殿失败"),
				new LanguageItem("105705", "攻击投石机失败"),
				new LanguageItem("105583", "攻城大败"),
				new LanguageItem("105703", "宫殿防守失败"),
				new LanguageItem("105581", "守城大败"),
				new LanguageItem("105707", "投石机防守失败"),
				new LanguageItem("105535", "由于您的实力与敌人差距过大，您的部队被全部消灭，战斗结果没有被送回来！"),
				new LanguageItem("105582", "攻城失败"),
				new LanguageItem("105580", "守城失败"),
				new LanguageItem("103715", "怪物报告"),
				new LanguageItem("108896", "很可惜，您慢了一步！该资源点已消失！"),
				new LanguageItem("105305", "系统"),
				new LanguageItem("115335", "{0}被摧毁了！"),
				new LanguageItem("115544", "国旗被摧毁通知"),
				new LanguageItem("105771", "头像上传成功"),
				new LanguageItem("101041", "优惠礼包"),
				new LanguageItem("101042", "感谢您使用GASH储值，这是赠送给您的礼包，祝您游戏愉快！"),
				new LanguageItem("103691", "编队功能解锁！"),
				new LanguageItem("133083", "活动结束"),
				new LanguageItem("115493", "捐献排名通告"),
				new LanguageItem("115494", "恭喜领主{0}、{1}、{2}荣登本周捐献排行榜前三名。" + "\n为感谢他们对联盟的发展作出的巨大贡献，将赠送他们若干物资作为奖励，望各位领主向其学习，踊跃捐献!"
						+ "\n以下是捐献排名奖励详情：" + "\n第一名：12K联盟荣誉、12K联盟积分、300K粮食、300K木材" + "\n第二名：10K联盟荣誉、10K联盟积分、200K粮食、200K木材"
						+ "\n第三名：8K联盟荣誉、8K联盟积分、100K粮食、100K木材"),
				new LanguageItem("115496", "捐献排名奖励"),
				new LanguageItem("105347", "你"),
				new LanguageItem("105387", "领主大人，您选择的邮件中已经没有奖励可以领取"),
				new LanguageItem("105388", "您确认要领取这些邮件中的全部奖励吗？"),
				new LanguageItem("105392", "举报消息"),
				new LanguageItem("105393", "确认举报该条消息？"),
				new LanguageItem("105394", "您的举报已经提交，感谢您为改善游戏内环境做出的贡献"),
				new LanguageItem("105395", "您已经举报过该条消息，请勿重复提交")

				// 原来wrapper中配置的
				,
				new LanguageItem("E100068", "您未加入联盟，暂时无法使用联盟聊天频道"),
				new LanguageItem("115020", "加入"),
				new LanguageItem("105207", "禁言"),
				new LanguageItem("105209", "已禁言"),
				new LanguageItem("105210", "是否禁言：{0}？"),
				new LanguageItem("105300", "国家"),
				new LanguageItem("105302", "发送"),
				new LanguageItem("105304", "复制"),
				new LanguageItem("105307", "您发布的聊天消息过于频繁，请稍等！"),
				new LanguageItem("105308", "发送邮件"),
				new LanguageItem("105309", "查看玩家"),
				new LanguageItem("105312", "屏蔽"),
				new LanguageItem("105313", "是否要屏蔽{0}，屏蔽后你将不会收到该玩家的任何聊天信息和邮件，但是你可以通过设置来解除对该玩家的屏蔽。"),
				new LanguageItem("105315", "解除屏蔽"),
				new LanguageItem("105316", "聊天"),
				new LanguageItem("105321", "由{0}翻译"),
				new LanguageItem("105322", "原文"),
				new LanguageItem("105502", "下滑加载更多"),
				new LanguageItem("105602", "联盟"),
				new LanguageItem("108584", "邀请加入联盟"),
				new LanguageItem("115922", "屏蔽该玩家留言"),
				new LanguageItem("115923", "屏蔽该联盟留言"),
				new LanguageItem("115925", "是否要屏蔽{0}，屏蔽后该玩家将无法对您的联盟进行留言，但是你可以通过联盟管理来解除对该玩家的屏蔽。"),
				new LanguageItem("115926", "是否要屏蔽{0}，屏蔽后该联盟将无法对您的联盟进行留言，但是你可以通过联盟管理来解除对该联盟的屏蔽。"),
				new LanguageItem("115929", "联盟留言"),
				new LanguageItem("115181", "系统"),
				new LanguageItem("115182", "我邀请了{0}加入我们的联盟，希望他能和我们一起并肩作战。"),
				new LanguageItem("115281", "查看战报"),
				new LanguageItem("115282", "抱歉，该战报已无法查看"),
				new LanguageItem("115168", "立即加入联盟获得金币"),
				new LanguageItem("115169", "{0}金币"),
				new LanguageItem("115170", "通过邮件发送"),
				new LanguageItem("105326", "翻译"),
				new LanguageItem("105327", "下拉可加载更多"),
				new LanguageItem("105328", "松开载入更多"),
				new LanguageItem("105324", "服务器即将在{0}分后停机更新"),
				new LanguageItem("105325", "服务器即将在{0}秒后停机更新"),
				new LanguageItem("115068", "立即加入"),
				new LanguageItem("confirm", "确定"),
				new LanguageItem("cancel_btn_label", "取消"),
				new LanguageItem("114110", "加载中"),
				new LanguageItem("104932", "刷新"),
				new LanguageItem("105564", "全体联盟成员"),
				new LanguageItem("101205", "邮件"),
				new LanguageItem("105329", "论坛"),
				new LanguageItem("105522", "侦察战报"),
				new LanguageItem("105591", "小时"),
				new LanguageItem("105330", "是否重发此消息?"),
				new LanguageItem("105332", "发送王国公告将消耗1个 {0}！"),
				new LanguageItem("104371", "号角"),
				new LanguageItem("105333", "领主大人，您的 {0} 不足，花费一些金币即可发送王国公告！"),
				new LanguageItem("104912", "领主大人，您的金币不足，赶快去购买一些吧！"),
				new LanguageItem("105331", "公告"),
				new LanguageItem("103001", "VIP{0}"),
				new LanguageItem("105352", "{0}条新消息"),
				new LanguageItem("105353", "以下为新消息"),
				new LanguageItem("105369", "以下是最近{0}条新消息"),
				new LanguageItem("105384", "信息"),
				new LanguageItem("105519", "战斗报告"),
				new LanguageItem("103731", "逐鹿天下游戏工作室"),
				new LanguageItem("132000", "联系MOD"),
				new LanguageItem("108523", "删除"),
				new LanguageItem("105569", "公告"),
				new LanguageItem("105516", "资源采集报告"),
				new LanguageItem("114121", "资源帮助报告"),
				new LanguageItem("103715", "怪物"),
				new LanguageItem("111660", "我刚刚在铁匠铺在成功的锻造出了{0}"),
				new LanguageItem("138039", "战场"),
				new LanguageItem("105504", "内容"),
				new LanguageItem("105505", "收件人"),
				new LanguageItem("105395", "您已经举报过该条信息了"),
				new LanguageItem("105394", "您的举报已经提交，感谢您为提升游戏内环境作出的贡献"),
				new LanguageItem("105393", "确认举报该条消息？"),
				new LanguageItem("132120", "标记已读"),
				new LanguageItem("105392", "举报消息"),
				new LanguageItem("132103", "好友列表"),
				new LanguageItem("132122", "欢迎欢迎"),
				new LanguageItem("132123", "欢迎{1}加入{0}"),
				new LanguageItem("132121", "打招呼"),
				new LanguageItem("105354", "多人聊天"),
				new LanguageItem("132124", "{0}欢迎你的加入，咱们一起好好玩"),
				new LanguageItem("134054", "{0}刚刚发布了联盟任务{1}，请大家前往协助"),
				new LanguageItem("134055", "{0}刚刚发布的联盟任务{1}已经被{2}接取"),
				new LanguageItem("104974", "点击领取"),
				new LanguageItem("104986", "点击查看"),
				new LanguageItem("104973", "各位朋友快来抢{0}啦"),
				new LanguageItem("104993", "拆红包"),
				new LanguageItem("104998", "发了一个{0}"),
				new LanguageItem("104978", "手慢了，红包已经被抢光了"),
				new LanguageItem("104975", "看看大家的手气"),
				new LanguageItem("160256", "该{0}已过期"),
				new LanguageItem("105110", "当前游戏版本不支持此消息类型"),
				new LanguageItem("132134", "巨龙战场中世界频道不可用"),
				new LanguageItem("104983", "红包") };
		LanguageManager.initChatLanguage(chatLangArray);
	}
}
