#include "CCScrollLayer.h"
#include <assert.h>

namespace cocos2d 
{
	enum 
	{
		kCCScrollLayerStateIdle,
		kCCScrollLayerStateSliding,
	}; 

	CCScrollLayer::CCScrollLayer()
		: m_pDelegate(NULL), m_pLayers(NULL), m_bStealingTouchInProgress(false), m_pScrollTouch(NULL), m_iState(kCCScrollLayerStateIdle), m_colorNormal(ccc4f(0x96,0x96,0x96,0xFF)), m_colorHighLight(ccc4f(0xFF,0xFF,0xFF,0xFF))

	{
	}

	CCScrollLayer::~CCScrollLayer()
	{
		CC_SAFE_RELEASE(m_pLayers);
		m_pDelegate = NULL;
	}

	unsigned int CCScrollLayer::getTotalScreens() const
	{
		return m_pLayers->count();
	}

	CCScrollLayer* CCScrollLayer::nodeWithLayers(CCArray* layers, int widthOffset)
	{
		CCScrollLayer* pRet = new CCScrollLayer();
		if (pRet && pRet->initWithLayers(layers, widthOffset))
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = NULL;
			return NULL;
		}
	}
		
	bool CCScrollLayer::initWithLayers(CCArray* layers, int widthOffset)
	{
		if (!CCLayer::init())
			return false;
		CC_ASSERT(layers && layers->count());
		
		setTouchEnabled(true);

		m_bStealTouches = true;
		
		// Set default minimum touch length to scroll.
		m_fMinimumTouchLengthToSlide = 30.0f;
		m_fMinimumTouchLengthToChangePage = 50.0f;
		
		m_fMarginOffset = CCDirector::sharedDirector()->getWinSize().width;

		// Show indicator by default.
		m_bShowPagesIndicator = true;
		m_tPagesIndicatorPosition = ccp(0.5f * _contentSize.width, ceilf(_contentSize.height / 8.0f));

		// Set up the starting variables
		m_uCurrentScreen = 0;	

		// Save offset.
		m_fPagesWidthOffset = (float)widthOffset;			

		// Save array of layers.
        m_pLayers = CCArray::createWithArray(layers);
		layers->release();
		m_pLayers->retain();

		updatePages();	

		return true;
	}

	void CCScrollLayer::updatePages()
	{
		// Loop through the array and add the screens if needed.
		int i = 0;
		CCObject* object = NULL;
		CCARRAY_FOREACH(m_pLayers, object)
		{
			CCLayer* layer = (CCLayer*)object;
			layer->setAnchorPoint(ccp(0,0));
            // caution: this will modify layer's contentSize (which is incompatible for scrollview)
			layer->setContentSize(CCDirector::sharedDirector()->getWinSize());
			
            layer->setPosition(ccp((i * (_contentSize.width - m_fPagesWidthOffset)), 0));
			if (!layer->getParent())
				addChild(layer);
			i++;
		}
	}

    static void drawSolidCircle(const CCPoint& center,float radius,unsigned int segments,ccColor4F color)
	{
		const float coef = 2.0f * (float)M_PI/segments;
		CCPoint *vertices=(CCPoint*)calloc(segments+1,sizeof(CCPoint));
        
		for(unsigned int i = 0;i <= segments; i++) {
			float rads = i*coef;
			GLfloat j = radius * cosf(rads)  + center.x;
			GLfloat k = radius * sinf(rads)  + center.y;
			vertices[i].x=j;
			vertices[i].y=k;
		}
		ccDrawSolidPoly( vertices, segments,color);
		free(vertices);
	}
	// CCLayer Methods ReImpl

	void CCScrollLayer::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags)
	{
		CCLayer::visit();	//< Will draw after glPopScene. 

		if (m_bShowPagesIndicator)
		{
			int totalScreens = getTotalScreens();

			// Prepare Points Array
			float n = (float)totalScreens; //< Total points count in float.
			float pY = m_tPagesIndicatorPosition.y; //< Points y-coord in parent coord sys.
			float d = 8.0f; //< Distance between points.
			for (int i = 0; i < totalScreens; ++i)
			{
				float pX = m_tPagesIndicatorPosition.x + d * ((float)i - 0.5f*(n-1.0f));
				;
                ccColor4F color = (i==m_uCurrentScreen)?m_colorHighLight:m_colorNormal;
                drawSolidCircle(ccp(pX, pY), 2, 4, color);
			}
		}
	}

	// Moving To / Selecting Pages

	void CCScrollLayer::moveToPageEnded()
	{
		if (m_pDelegate)
			m_pDelegate->scrollLayerScrolledToPageNumber(this, m_uCurrentScreen);
	}

	unsigned int CCScrollLayer::pageNumberForPosition(const CCPoint& position)
	{
		float pageFloat = - _position.x / (_contentSize.width - m_fPagesWidthOffset);
		int pageNumber = (int)ceilf(pageFloat);
		if ((float)pageNumber - pageFloat  >= 0.5f)
			pageNumber--;

		pageNumber = MAX(0, pageNumber);
		pageNumber = MIN((int)m_pLayers->count() - 1, pageNumber);

		return (unsigned int)pageNumber;
	}


	CCPoint CCScrollLayer::positionForPageWithNumber(unsigned int pageNumber)
	{
		return ccp(pageNumber * -1.f * (_contentSize.width - m_fPagesWidthOffset), 0.0f);
	}

	void CCScrollLayer::moveToPage(unsigned int pageNumber)
	{	
		if (pageNumber >= m_pLayers->count()) 
		{
			CCLOGERROR("CCScrollLayer::moveToPage: %d - wrong page number, out of bounds.", pageNumber);
			return;
		}

		CCAction* changePage =
        CCSequence::createWithTwoActions(CCMoveTo::create(0.3f, positionForPageWithNumber(pageNumber)),
                                         CCCallFunc::create(this, callfunc_selector(CCScrollLayer::moveToPageEnded)));

		runAction(changePage);
		m_uCurrentScreen = pageNumber;
	}

	void CCScrollLayer::selectPage(unsigned int pageNumber)
	{
		if (pageNumber >= m_pLayers->count()) 
		{
			CCLOGERROR("CCScrollLayer::selectPage: %d - wrong page number, out of bounds.", pageNumber);
			return;
		}

		setPosition(positionForPageWithNumber(pageNumber));
		m_uCurrentScreen = pageNumber;

	}

	// Dynamic Pages Control

	void CCScrollLayer::addPage(CCLayer* aPage)
	{
		addPage(aPage, m_pLayers->count());
	}

	void CCScrollLayer::addPage(CCLayer* aPage, unsigned int pageNumber)
	{
		pageNumber = MIN(pageNumber, m_pLayers->count());
		pageNumber = MAX(pageNumber, 0);

		m_pLayers->insertObject(aPage, pageNumber);

		updatePages();
		moveToPage(m_uCurrentScreen);
	}

	void CCScrollLayer::removePage(CCLayer* aPage)
	{
		m_pLayers->removeObject(aPage);
		removeChild(aPage, true);

		updatePages();

		m_uCurrentScreen = MIN(m_uCurrentScreen, m_pLayers->count() - 1);
		moveToPage(m_uCurrentScreen);
	}

	void CCScrollLayer::removePageWithNumber(unsigned int pageNumber)
	{
		if (pageNumber < m_pLayers->count())
			removePage((CCLayer*)(m_pLayers->objectAtIndex(pageNumber)));
	}

	// Touches

	// Register with more priority than CCMenu's but don't swallow touches /*YOU DO NOT NEED SO HIGH PRIORITY -- change to 1 by ZouC*/
//	void CCScrollLayer::registerWithTouchDispatcher()
//	{
//		//CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
//	}

	/** Hackish stuff - stole touches from other CCTouchDispatcher targeted delegates. 
	 Used to claim touch without receiving ccTouchBegan. */
	void CCScrollLayer::claimTouch(CCTouch* pTouch)
	{
//		CCTargetedTouchHandler* handler = (CCTargetedTouchHandler*)//CCDirector::sharedDirector()->getTouchDispatcher()->findHandler(this);		
//		if (handler)
//		{
//			CCSet* claimedTouches = handler->getClaimedTouches();
//			if (!claimedTouches->containsObject(pTouch))
//			{
//				claimedTouches->addObject(pTouch);
//			}
//			else 
//			{
//				CCLOGERROR("CCScrollLayer::claimTouch is already claimed!");
//			}
//		}
	}

	void CCScrollLayer::cancelAndStoleTouch(CCTouch* pTouch, CCEvent* pEvent)
	{
		// Throw Cancel message for everybody in TouchDispatcher.
		CCSet* touchSet = new CCSet();
		touchSet->addObject(pTouch);
		touchSet->autorelease();
		m_bStealingTouchInProgress = true;
		//CCDirector::sharedDirector()->getTouchDispatcher()->touchesCancelled(touchSet, pEvent);
		m_bStealingTouchInProgress = false;

		//< after doing this touch is already removed from all targeted handlers

		// Squirrel away the touch
		claimTouch(pTouch);
	}

	void CCScrollLayer::onTouchCancelled(CCTouch* pTouch, CCEvent* pEvent)
	{
		// Do not cancel touch, if this method is called from cancelAndStoleTouch:
		if (m_bStealingTouchInProgress)
			return;

		if (m_pScrollTouch == pTouch)
		{
			m_pScrollTouch = NULL;
			selectPage(m_uCurrentScreen);
		}
	}

	bool CCScrollLayer::onTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
	{
		if (!m_pScrollTouch)
			m_pScrollTouch = pTouch;
		else
			return false;
		CCPoint touchPoint = pTouch->getLocation();
		touchPoint = CCDirector::sharedDirector()->convertToGL(pTouch->getLocation());

		m_fStartSwipe = touchPoint.x;
		m_iState = kCCScrollLayerStateIdle;
		
		return true;
	}

	void CCScrollLayer::onTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
	{
		if(m_pScrollTouch != pTouch)
			return;

        CCPoint touchPoint = pTouch->getLocation();
		touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);

		// If finger is dragged for more distance then minimum - start sliding and cancel pressed buttons.
		// Of course only if we not already in sliding mode
		if ((m_iState != kCCScrollLayerStateSliding) 
			&& (fabsf(touchPoint.x - m_fStartSwipe) >= m_fMinimumTouchLengthToSlide))
		{
			m_iState = kCCScrollLayerStateSliding;

			// Avoid jerk after state change.
			m_fStartSwipe = touchPoint.x;

			if (m_bStealTouches)
				cancelAndStoleTouch(pTouch, pEvent);

			if (m_pDelegate)
				m_pDelegate->scrollLayerScrollingStarted(this);
		}

		if (m_iState == kCCScrollLayerStateSliding)
		{
			float desiredX = (m_uCurrentScreen * -1.f * (_contentSize.width - m_fPagesWidthOffset)) + touchPoint.x - m_fStartSwipe;
			unsigned int page = pageNumberForPosition(ccp(desiredX, 0));
			float offset = desiredX - positionForPageWithNumber(page).x;
			if ((page == 0 && offset > 0) || (page == m_pLayers->count() - 1 && offset < 0))
				offset -= m_fMarginOffset * (offset / CCDirector::sharedDirector()->getWinSize().width);
			else
				offset = 0;
			setPosition(ccp(desiredX - offset, 0));
		}
	}

	void CCScrollLayer::onTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
	{
		if(m_pScrollTouch != pTouch)
			return;

		m_pScrollTouch = NULL;

        CCPoint touchPoint = pTouch->getLocation();
		touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);

		unsigned int selectedPage = m_uCurrentScreen;
		float delta = touchPoint.x - m_fStartSwipe;
		if (fabs(delta) >= m_fMinimumTouchLengthToChangePage)
		{
			selectedPage = pageNumberForPosition(_position);
			if (selectedPage == m_uCurrentScreen)
			{
				if (delta < 0.f && selectedPage < m_pLayers->count() - 1)
					selectedPage++;
				else if (delta > 0.f && selectedPage > 0)
					selectedPage--;
			}
		}
		moveToPage(selectedPage);
	}
}


