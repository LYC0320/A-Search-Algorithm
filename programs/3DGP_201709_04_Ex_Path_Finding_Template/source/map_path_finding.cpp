#include "map.h"
using namespace std;

void MAP::setInvisibleAllPathPoints( )
{
    for ( int j = 0; j < mMaxNumOfPathPoints; ++j ) {
        mSceneNodes_Path[j]->setVisible( false );
    }
}

//
// Compute the grid cell indices (ix, iz) for input point p.
// The grid is defined on the x-z plane.
//
void MAP::computeIndices( const Vector3 &p, int &ix, int &iz ) const
{
    Vector3 relative_p = p - mMinSpace;
	
    ix = (relative_p.x + mDWidth.x*0.5 )/ mDWidth.x ;
    iz = (relative_p.z + mDWidth.z*0.5 ) / mDWidth.z;

	
    if ( ix < 0 ) ix = 0;
    if ( ix >= mNX - 1 ) ix = mNX - 1;

    if ( iz < 0 ) iz = 0;
    if ( iz >= mNZ - 1 ) iz = mNZ - 1;
	
	
}

Vector3 MAP::computePointPosition( int ix, int iz ) const
{
    Real x, z;
    x = ix*mDWidth.x + mMinSpace.x;
    z = iz*mDWidth.z + mMinSpace.z;
    return Vector3( x, 0.0, z );

}

void MAP::setPathPoint( int index, const pair<int , int> &point )
{
    Vector3 p = computePointPosition( point.first, point.second );
    mSceneNodes_Path[index]->setPosition( p );
    mSceneNodes_Path[index]->setVisible( true );
}

void MAP::setPathPoint( int index, const Vector3 &position )
{
    mSceneNodes_Path[index]->setPosition( position );
    mSceneNodes_Path[index]->setVisible( true );
}

//
// S <- A <- B <- G
// Use pre-traversal to get: G, B, A, S
//
// Traverse the linked list using the parent pointer.
//
//  if ( node == 0 ) return;
//  constructPathPoints( node->parent );
//  mPathPoints.push_back( node->position );
//
void MAP::constructPathPoints( MapNode *node )
{
    if ( node == 0 ) return;
    constructPathPoints( node->parent );

	//cout << node->ix << "," << node->iz << endl;

    mPathPoints.push_back( node->position );
	pathNode.push_back(node);

	//cout << node->position<<endl;

}

/*
void MAP::constructPathPoints( int goal_ix, int goal_iz )
{
    mPathPoints.clear( );
    MapNode *goalNode = &mMapNodes[goal_ix][goal_iz];
    constructPathPoints( goalNode );
    
}
*/

std::vector<Vector3> MAP::getPathPoints( ) const
{
    return mPathPoints;
}

double MAP::getCostBetweenTwoNodes(const MapNode *p0, const MapNode *p1) const
{
	if (!(p1->flgCanMove))
	{
		return DBL_MAX;
	}

	return sqrt((p0->position.x - p1->position.x, 2) + pow(p0->position.y - p1->position.y, 2));
}

MapNode *MAP::getNodeFromOpenList()
{
	double minValue = DBL_MAX;
	int minIndex;
	MapNode *lowestNode;

	for (int i = 0; i < mOpenList.size(); i++)
	{
		if (mOpenList[i]->f < minValue)
		{
			minValue = mOpenList[i]->f;
			lowestNode = mOpenList[i];
		}	
	}

	return lowestNode;
}

void MAP::removeNodeFromOpenList(MapNode *node)
{
	for (int i = 0; i < mOpenList.size(); i++)
	{
		if (mOpenList[i] == node) 
		{
			mOpenList.erase(mOpenList.begin() + i);
			//cout << "remove" << endl;
		}
	}
}

void MAP::computeNeighbor(MapNode *node, std::vector<MapNode *> &neightbors)
{
	if (node->ix - 1 >= 0)
		neightbors.push_back(&mMapNodes[node->ix - 1][node->iz]);
	if (node->iz - 1 >= 0)
		neightbors.push_back(&mMapNodes[node->ix][node->iz - 1]);
	if (node->ix + 1 <= 19)
		neightbors.push_back(&mMapNodes[node->ix + 1][node->iz]);
	if (node->iz + 1 <= 19)
		neightbors.push_back(&mMapNodes[node->ix][node->iz + 1]);
	
}

void MAP::clearAllCosts()
{
	for (int j = 0; j < mNZ; ++j)
	{
		for (int i = 0; i < mNX; ++i) 
		{
			mMapNodes[i][j].g = 0;
			mMapNodes[i][j].h = 0;
			mMapNodes[i][j].f = 0;
			mMapNodes[i][j].parent = NULL;
		}
	}
}


//
// Implement the A* algorithm
//
void MAP::performAStarPathFinding( ) 
{
	//cout << mMapNodes[9][9].position << endl;

    cout << "here" << endl;
    mOpenList.clear( );
    mClosedList.clear( );
	pathNode.clear();
	mPathPoints.clear();
    clearAllCosts( );

	mOpenList.push_back(mStartNode);
	mStartNode->g = 0;
	mStartNode->h = getCostBetweenTwoNodes(mStartNode, mGoalNode);
	mStartNode->f = mStartNode->g + mGoalNode->h;

	
	

	/*
	cout << mOpenList.empty() << endl;

	current = getNodeFromOpenList();

	removeNodeFromOpenList(current);
	cout << mOpenList.empty() << endl;
	*/

	
	while (!mOpenList.empty())
	{
		//cout << mOpenList.size() << endl;
		MapNode *current;
		std::vector <MapNode*>neighbors;

		current = getNodeFromOpenList();

		if (current == mGoalNode)
		{
			//mStartNode->setParent(NULL);

			cout << "there" << endl;

			constructPathPoints(mGoalNode);

			//cout << "13213216sdfsdfsdfsdf5" << endl;

			break;
		}

		removeNodeFromOpenList(current);

		//cout << mOpenList.size() << endl;

		mClosedList.push_back(current);

		computeNeighbor(current, neighbors);

		for (int i = 0; i < neighbors.size(); i++)
		{
			bool checkClosedList = false;
			bool checkOpenList = false;
			double tentiveScore;
			bool tentiveIsBetter;

			//cout << "1" << endl;

			for (int j = 0; j < mClosedList.size(); j++)
			{
				if (neighbors[i] == mClosedList[j])
				{
					checkClosedList = true;
				}
			}

			//cout << "2" << endl;

			if (checkClosedList)
			{
				//tentiveScore = current->g + getCostBetweenTwoNodes(current, neighbors[i]);
				continue;
			}

			tentiveScore = current->g + getCostBetweenTwoNodes(current, neighbors[i]);

			for (int j = 0; j < mOpenList.size(); j++)
			{
				if (neighbors[i] == mOpenList[j])
				{
					checkOpenList = true;
				}
			}

			//cout << "3" << endl;

			if (!checkOpenList)
			{
				mOpenList.push_back(neighbors[i]);
				tentiveIsBetter = true;
			}
			else if (tentiveScore < neighbors[i]->g)
			{
				tentiveIsBetter = true;
			}
			else
			{
				tentiveIsBetter = false;
			}

			if (tentiveIsBetter)
			{
				neighbors[i]->setParent(current);
				neighbors[i]->g = tentiveScore;
				neighbors[i]->h = getCostBetweenTwoNodes(neighbors[i], mGoalNode);
				neighbors[i]->f = neighbors[i]->g + neighbors[i]->h;
			}

			//cout << "4" << endl;
			
		}

		neighbors.clear();

		//cout << "5" << endl;


	}
	
    
    double g, h;
    g = 0.0;
    //h = getCostBetweenTwoNodes( mStartNode, mGoalNode );
}

bool MAP::performPathFinding(const Vector3 &p, const Vector3 &q)
{

	//Why?
	myStart.position = p;
	myGoal.position = q;

	mStartNode = &myStart;
	mGoalNode = &myGoal;

	computeIndices(mStartNode->position, mStartNode->ix, mStartNode->iz);
	computeIndices(mGoalNode->position, mGoalNode->ix, mGoalNode->iz);

	mStartNode = &mMapNodes[mStartNode->ix][mStartNode->iz];
	mGoalNode = &mMapNodes[mGoalNode->ix][mGoalNode->iz];


	if(mGoalNode->flgCanMove)
		performAStarPathFinding();

	//cout << mStartNode->ix << "," << mStartNode->iz << endl;
	//cout << mGoalNode->ix << "," << mGoalNode->iz << endl;

	for (int j = 0; j < mNZ; ++j)
	{
		for (int i = 0; i < mNX; ++i) 
		{
			int index = i + j * mNX;
			mSceneNodes_Path[index]->setVisible(false);
		}
	}

	for (int i = 0; i < pathNode.size(); i++)
	{
		int index = pathNode[i]->ix + pathNode[i]->iz * mNX;
		mSceneNodes_Path[index]->setVisible(true);
	}

	
	

	return true;
}