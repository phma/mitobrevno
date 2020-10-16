/******************************************************/
/*                                                    */
/* itree.h - interval tree                            */
/*                                                    */
/******************************************************/
/* Copyright 2020 Pierre Abbat.
 * This file is part of Mitobrevno.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/* The interval tree is made by plotting start time against end time and
 * splitting the resulting isosceles right triangle, whose hypotenuse contains
 * the instant intervals, as follows:
 * If the side is 0, it is empty.
 * If the side is 1, it contains one interval.
 * If the side is 2n, it is split into 3 triangles of side n and 1 of side n-1.
 * If the side is 2n+1, it is split into 3 triangles of side n and 1 of side n+1.
 *
 *  *  *  *  *  *  *  *  *
 *  *  *  *  *  *  *  *
 *  *  *  *  *  *  *
 *  *  *  *  *  *
 *  *  *  *  *
 *  *  *  *
 *  *  *
 *  *
 */
