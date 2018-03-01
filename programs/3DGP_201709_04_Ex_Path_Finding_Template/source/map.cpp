#include "map.h"

MAP::MAP( )
{
    mNX = 20;
    mNZ = 20;
    mMinSpace = Vector3( -250.0, 0.0, -250.0 );
    mMaxSpace = Vector3( 250.0, 0.0, 250.0 );
    mSceneNodes = 0;
    mEntitys = 0;
    reset( );
}

void MAP::setDimension( int nx, int nz )
{
    mNX = nx;
    mNZ = nz;
    reset( );
}

void MAP::reset( )
{
    for ( int j = 0; j < MAX_NZ; ++j ) {
        for ( int i = 0; i < MAX_NX; ++i ) {
            mMapNodes[i][j].flgCanMove = false;
        }
    }
}

void MAP::generateMap( SceneManager* mSceneMgr )
{
    mSceneNodes = new SceneNode*[mNX*mNZ];
	mSceneNodes_Path = new SceneNode*[mNX*mNZ];
    mEntitys = new Entity*[mNX*mNZ];
	mEntitys_Path = new Entity*[mNX*mNZ];
    Vector3 w = mMaxSpace - mMinSpace;
    double dx = w.x/(mNX-1);
    double dz = w.z/(mNZ-1);
    //
    mDWidth = Vector3( dx, 0.0, dz );

    float size = 0.5;
    Vector3 scales = size*Vector3(0.35, 1.0, 0.35);
	Vector3 ballScales = size*Vector3(1.0, 1.0, 1.0)*0.6;
    float translate_y = -size*40;
    for ( int j = 0; j < mNZ; ++j ) 
	{
        for ( int i = 0; i < mNX; ++i ) 
		{
            int index = i + j*mNX;
            double x, y, z;
            y = 0.0;
            x = i*dx + mMinSpace.x;
            z = j*dz + mMinSpace.z;
            //

			
				mMapNodes[i][j].flgCanMove = false;
				mMapNodes[i][j].ix = i;
				mMapNodes[i][j].iz = j;

				mMapNodes[i][j].position = Vector3(x, 0.0, z);
				//
				mSceneNodes[index] = mSceneMgr->getRootSceneNode()->createChildSceneNode();
				mSceneNodes[index]->setPosition(Vector3(x, y, z));
				mSceneNodes[index]->scale(scales.x, scales.y, scales.z);
				mSceneNodes[index]->translate(0.0, translate_y, 0.0);

				mSceneNodes_Path[index] = mSceneMgr->getRootSceneNode()->createChildSceneNode();
				mSceneNodes_Path[index]->setPosition(Vector3(x, y, z));
				mSceneNodes_Path[index]->scale(ballScales.x, ballScales.y, ballScales.z);
				//mSceneNodes_Path[index]->translate(0.0, translate_y, 0.0);
				
				
				String nameBall;
				genNameUsingIndex("ball", index, nameBall);


				mEntitys_Path[index] = mSceneMgr->createEntity(nameBall,Ogre::SceneManager::PT_SPHERE);
				mEntitys_Path[index]->setMaterialName("Examples/gray");
				mSceneNodes_Path[index]->attachObject(mEntitys_Path[index]);

				mSceneNodes_Path[index]->setVisible(false);

				String name;
				genNameUsingIndex("br", index, name);

				
				mEntitys[index] = mSceneMgr
					->createEntity(name, "cube.mesh");
				mEntitys[index]->setMaterialName("Examples/SphereMappedRustySteel");
				mSceneNodes[index]->attachObject(mEntitys[index]);
				
        }

         
    }
    //
    

	
    for ( int j = 1; j < mNZ-1; ++j ) 
	{
        for ( int i = 1; i < mNX-1; ++i ) 
		{
			int index = i + j*mNX;

			if (j != 2 && j != 7 && j != 14 && j != 17)
			{
				if (index != 82 && index != 102)
				{
					mSceneNodes[index]->setVisible(false);
					mMapNodes[i][j].flgCanMove = true;
				}
			}

			if (index == 41 || index == 58 || index == 141 || index == 158 || index == 298 || index == 341)
			{
				mSceneNodes[index]->setVisible(false);
				mMapNodes[i][j].flgCanMove = true;
			}
        }
    }
    

}