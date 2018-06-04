//
// @author Yurii Shyrma (iuriish@yahoo.com), created on 04.06.2018
//


#include <ops/declarable/CustomOperations.h>


namespace nd4j    {
namespace ops     {

//////////////////////////////////////////////////////////////////////////
CUSTOM_OP_IMPL(reduce_variance, 1, 1, false, 0, 0) {

    NDArray<T> *input   = INPUT_VARIABLE(0);     

    NDArray<T> *output  = OUTPUT_VARIABLE(0);

    const bool keepDims = block.getTArguments()->size() > 0 ? (bool)T_ARG(0) : false;
    
    std::vector<int> dimensions = *block.getIArguments();    

    REQUIRE_TRUE(dimensions.size() <= input->rankOf(), 0, "REDUCE_VARIANCE OP: the number of dimensions to reduce along must be <= input array rank, but got %i instead" , dimensions.size());

    for(const auto& item : dimensions)
        REQUIRE_TRUE(item > -input->rankOf() || item < input->rankOf(), 0, "REDUCE_VARIANCE OP: the input dimension to reduce along must be in range (-%i, %i), but got %i instead !" , input->rankOf(), input->rankOf(), item);
        
    input->template varianceAlongDimension<simdOps::SummaryStatsVariance<T>>(output, false, dimensions);    // biased variance

    return Status::OK();
}


DECLARE_SHAPE_FN(reduce_variance) {    

    const bool keepDims = block.getTArguments()->size() > 0 ? (bool)T_ARG(0) : false;
    
    std::vector<int> dimensions = *block.getIArguments();

    REQUIRE_TRUE(dimensions.size() <= inputShape->at(0)[0], 0, "REDUCE_VARIANCE OP: the number of dimensions to reduce along must be <= input array rank, but got %i instead" , dimensions.size());
    
    for(const auto& item : dimensions)
        REQUIRE_TRUE(item > -inputShape->at(0)[0] || item < inputShape->at(0)[0], 0, "REDUCE_VARIANCE OP: the input dimension to reduce along must be in range (-%i, %i), but got %i instead !" , inputShape->at(0)[0], inputShape->at(0)[0], item);

    Nd4jLong* outShapeInfo = ShapeUtils<T>::evalReduceShapeInfo(shape::order(inputShape->at(0)), dimensions, inputShape->at(0), keepDims, false, block.getWorkspace());

    return SHAPELIST(outShapeInfo);
}



//////////////////////////////////////////////////////////////////////////
CUSTOM_OP_IMPL(reduce_variance_bp, 2, 1, false, 0, 0) {

    NDArray<T> *input  = INPUT_VARIABLE(0);
    NDArray<T> *gradO  = INPUT_VARIABLE(1);

    NDArray<T> *gradI  = OUTPUT_VARIABLE(0);

    const bool keepDims = block.getTArguments()->size() > 0 ? (bool)T_ARG(0) : false;
    
    std::vector<int> dimensions = *block.getIArguments();    

    REQUIRE_TRUE(dimensions.size() <= input->rankOf(), 0, "REDUCE_VARIANCE OP: the number of dimensions to reduce along must be <= input array rank, but got %i instead" , dimensions.size());

    for(const auto& item : dimensions)
        REQUIRE_TRUE(item > -input->rankOf() || item < input->rankOf(), 0, "REDUCE_VARIANCE OP: the input dimension to reduce along must be in range (-%i, %i), but got %i instead !" , input->rankOf(), input->rankOf(), item);    
    
    
    if(gradO->isScalar()) {
        
        *gradI = (*gradO)(0) / input->lengthOf();            
    }
    else {
        
        Nd4jLong* gradOShapeKeepDims = ShapeUtils<T>::evalReduceShapeInfo(input->ordering(), dimensions, *input, true, false, block.getWorkspace());
        NDArray<T>* gradOReshaped = gradO->reshape(gradO->ordering(), ShapeUtils<T>::pullShapeFromShapeInfo(gradOShapeKeepDims));  // for example could be something like [a,b] -> [1,a,1,b]

        *gradI = (static_cast<T>(gradO->lengthOf()) / input->lengthOf());        
        *gradI *= *gradOReshaped;                                                   // automatic broadcasting happens during this multiplication

        delete gradOReshaped;
    }

    return Status::OK();
}



DECLARE_SHAPE_FN(reduce_variance_bp) {    

    const bool keepDims = block.getTArguments()->size() > 0 ? (bool)T_ARG(0) : false;

    std::vector<int> dimensions = *block.getIArguments();

    REQUIRE_TRUE(dimensions.size() <= inputShape->at(0)[0], 0, "REDUCE_VARIANCE OP: the number of dimensions to reduce along must be <= input array rank, but got %i instead" , dimensions.size());
    
    for(const auto& item : dimensions)
        REQUIRE_TRUE(item > -inputShape->at(0)[0] || item < inputShape->at(0)[0], 0, "REDUCE_VARIANCE OP: the input dimension to reduce along must be in range (-%i, %i), but got %i instead !" , inputShape->at(0)[0], inputShape->at(0)[0], item);
    
    Nd4jLong* gradIshapeInfo(nullptr);
    COPY_SHAPE(inputShape->at(0), gradIshapeInfo);
        
    return SHAPELIST(gradIshapeInfo);
}


}
}
