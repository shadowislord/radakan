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

import com.radakan.util.preview.IAnimationHandler;

/**
 * AnimationUnit is simply an aggregation of Unit and AnimationHandler interfaces.
 * An animation unit allows one to change the animation of the entity, modify it's speed, or blend between animations.
 * 
 * @author Kirill Vainer
 */
public interface IAnimationUnit extends IUnit, IAnimationHandler {
}
