package com.elex.chatservice.model.mail.ocupy;

import org.apache.commons.lang.StringUtils;

import com.alibaba.fastjson.JSON;
import com.elex.chatservice.controller.JniController;
import com.elex.chatservice.model.LanguageKeys;
import com.elex.chatservice.model.LanguageManager;
import com.elex.chatservice.model.MailManager;
import com.elex.chatservice.model.mail.MailData;
import com.elex.chatservice.util.LogUtil;

public class OcupyMailData extends MailData
{
	private OcupyMailContents	detail;

	public OcupyMailContents getDetail()
	{
		return detail;
	}

	public void setDetail(OcupyMailContents detail)
	{
		this.detail = detail;
	}

	public void parseContents()
	{
		super.parseContents();
		if (!getContents().equals(""))
		{
			try
			{
				detail = JSON.parseObject(getContents(), OcupyMailContents.class);
				hasMailOpend = true;
				if (detail == null || needParseByForce)
					return;
				else
				{
					switch (detail.getPointType())
					{
						case MailManager.Throne:
						{
							nameText = LanguageManager.getLangByKey(LanguageKeys.MAIL_OCCUPY_PALACE);
							contentText = LanguageManager.getLangByKey(LanguageKeys.MAIL_OCCUPY_PALACE_SUCESS);
							break;
						}
						case MailManager.Trebuchet:
						{
							nameText = LanguageManager.getLangByKey(LanguageKeys.MAIL_OCCUPY_CATAPULT);
							contentText = LanguageManager.getLangByKey(LanguageKeys.MAIL_OCCUPY_CATAPULT_SUCESS);
							break;
						}
						default:
						{
							String cordX = "";
							String cordY = "";
							String pt = JniController.getInstance().excuteJNIMethod("getPointByIndex",
									new Object[] { Integer.valueOf(detail.getPointId()) });
							if (detail.getServerType() == MailManager.SERVER_BATTLE_FIELD)
								pt = JniController.getInstance().excuteJNIMethod("getPointByMapTypeAndIndex",
										new Object[] { Integer.valueOf(detail.getPointId()), Integer.valueOf(detail.getServerType()) });
							if (StringUtils.isNotEmpty(pt))
							{
								String[] cord = pt.split("_");
								if (cord.length == 2)
								{
									cordX = cord[0];
									cordY = cord[1];
								}
							}

							if (detail.isTreasureMap())
							{
								nameText = LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_111504);
								contentText = LanguageManager.getLangByKey(LanguageKeys.MAIL_TITLE_111506, "", cordX, cordY);
							}
							else
							{
								nameText = LanguageManager.getLangByKey(LanguageKeys.MAIL_OCCUPY_SUCESS);
								contentText = LanguageManager.getLangByKey(LanguageKeys.MAIL_OCCUPY_POINT, "", cordX, cordY);
							}

							break;
						}
					}
					if (contentText.length() > 50)
					{
						contentText = contentText.substring(0, 50);
						contentText = contentText + "...";
					}
				}

			}
			catch (Exception e)
			{
				LogUtil.trackMessage("[OcupyMailContents parseContents error]: contents:" + getContents());
			}
		}
	}
}
