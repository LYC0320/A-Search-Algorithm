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
    mPathPoints.push_back( node->position );
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

//
// Implement the A* algorithm
//
void MAP::performAStarPathFinding( ) 
{
    cout << "here" << endl;
    mOpenList.clear( );
    mClosedList.clear( );
    //clearAllCosts( );
    
    double g, h;
    g = 0.0;
    //h = getCostBetweenTwoNodes( mStartNode, mGoalNode );
}

bool MAP::performPathFinding(const Vector3 &p, const Vector3 &q)
{
	return true;
}