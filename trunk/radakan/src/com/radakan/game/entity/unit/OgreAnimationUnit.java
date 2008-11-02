/*
 * Radakan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Radakan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Radakan.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.radakan.game.entity.unit;

import com.jme.util.export.JMEExporter;
import com.jme.util.export.JMEImporter;
import com.radakan.game.entity.Entity;
import com.radakan.jme.mxml.anim.MeshAnimationController;
import com.radakan.util.preview.OgreAnimationHandler;
import java.io.IOException;
import java.io.PrintStream;
import org.w3c.dom.Node;

public class OgreAnimationUnit extends OgreAnimationHandler implements IAnimationUnit, IUnit {

    public OgreAnimationUnit(MeshAnimationController control){
        super(control);
    }
    
    @Override
    public void exportXML(PrintStream stream) {
    }

    @Override
    public void importXML(Node rootEntityNode) {
    }

    @Override
    public void update(float tpf) {
    }

    @Override
    public void attach(Entity entity) {
    }

    @Override
    public void detach() {
    }

    @Override
    public int getType() {
        return ANIMATION;
    }

    @Override
    public void write(JMEExporter ex) throws IOException {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public void read(JMEImporter im) throws IOException {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public Class getClassTag() {
        return IAnimationUnit.class;
    }

}
