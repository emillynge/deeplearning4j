/*******************************************************************************
 * Copyright (c) 2015-2018 Skymind, Inc.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Apache License, Version 2.0 which is available at
 * https://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 ******************************************************************************/

package org.nd4j.linalg.api.ops.impl.transforms;

import org.nd4j.autodiff.samediff.SDVariable;
import org.nd4j.autodiff.samediff.SameDiff;
import org.nd4j.linalg.api.ndarray.INDArray;
import org.nd4j.linalg.api.ops.BaseTransformOp;

import java.util.Collections;
import java.util.List;

/**
 * IsFinite function
 *
 * @author raver119@gmail.com
  */
public class IsFinite extends BaseTransformOp {
    public IsFinite(SameDiff sameDiff, SDVariable i_v, boolean inPlace) {
        super(sameDiff, i_v, inPlace);
    }

    public IsFinite(SameDiff sameDiff, SDVariable i_v, int[] shape, boolean inPlace, Object[] extraArgs) {
        super(sameDiff, i_v, shape, inPlace, extraArgs);
    }

    public IsFinite(SameDiff sameDiff, SDVariable i_v, Object[] extraArgs) {
        super(sameDiff, i_v, extraArgs);
    }

    public IsFinite() {}

    public IsFinite(INDArray x, INDArray z) {
        super(x, z);
    }

    public IsFinite(INDArray x, INDArray z, long n) {
        super(x, z, n);
    }

    public IsFinite(INDArray x, INDArray y, INDArray z, long n) {
        super(x, y, z, n);
    }

    public IsFinite(INDArray x) {
        super(x);
    }

    @Override
    public int opNum() {
        return 81;
    }

    @Override
    public String opName() {
        return "isfinite";
    }

    @Override
    public String onnxName() {
        return "IsFinite";
    }

    @Override
    public String tensorflowName() {
        return "IsFinite";
    }



    @Override
    public List<SDVariable> doDiff(List<SDVariable> i_v) {
        return Collections.singletonList(f().zerosLike(arg()));
    }

}
