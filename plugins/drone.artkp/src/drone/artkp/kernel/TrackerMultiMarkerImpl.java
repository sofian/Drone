/* The native implementation of the TrackerMultiMarker.
 *  
 * Copyright (c) 2006 Jean-Sebastien Senecal (js@drone.ws)
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
package drone.artkp.kernel;

public class TrackerMultiMarkerImpl extends TrackerMultiMarker {

 	public TrackerMultiMarkerImpl() {
		super();
	}

	public TrackerMultiMarkerImpl(int width, int height) {
		super();
		changeCameraSize(width, height);
	}
	
	protected native long _createTrackerHandle();

	static {
		System.loadLibrary("TrackerMultiMarkerImpl");
	}
}