/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
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

#include "otbISRAUnmixingImageFilter.h"

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorImageToMatrixImageFilter.h"
#include "otbStandardWriterWatcher.h"

const unsigned int Dimension = 2;
typedef double PixelType;

typedef otb::VectorImage<PixelType, Dimension> ImageType;
typedef otb::ImageFileReader<ImageType> ReaderType;
typedef otb::ISRAUnmixingImageFilter<ImageType, ImageType, PixelType> UnmixingImageFilterType;
typedef otb::VectorImageToMatrixImageFilter<ImageType> VectorImageToMatrixImageFilterType;
typedef otb::ImageFileWriter<ImageType> WriterType;

int otbISRAUnmixingImageFilterNewTest(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  UnmixingImageFilterType::Pointer filter = UnmixingImageFilterType::New();
  std::cout << filter << std::endl;
  return EXIT_SUCCESS;
}

int otbISRAUnmixingImageFilterTest(int itkNotUsed(argc), char * argv[])
{
  const char * inputImage = argv[1];
  const char * inputEndmembers = argv[2];
  const char * outputImage = argv[3];
  int maxIter = atoi(argv[4]);

  ReaderType::Pointer readerImage = ReaderType::New();
  readerImage->SetFileName(inputImage);

  ReaderType::Pointer readerEndMembers = ReaderType::New();
  readerEndMembers->SetFileName(inputEndmembers);
  VectorImageToMatrixImageFilterType::Pointer endMember2Matrix = VectorImageToMatrixImageFilterType::New();
  endMember2Matrix->SetInput(readerEndMembers->GetOutput());

  endMember2Matrix->Update();

  typedef VectorImageToMatrixImageFilterType::MatrixType MatrixType;
  MatrixType endMembers = endMember2Matrix->GetMatrix();
  MatrixType pinv = vnl_matrix_inverse<PixelType>(endMembers);

  UnmixingImageFilterType::Pointer unmixer = UnmixingImageFilterType::New();

  unmixer->SetInput(readerImage->GetOutput());
  unmixer->SetMaxIteration(maxIter);
  //unmixer->SetNumberOfThreads(1);
  unmixer->SetEndmembersMatrix(endMember2Matrix->GetMatrix());

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputImage);
  writer->SetInput(unmixer->GetOutput());
  writer->SetNumberOfDivisionsStrippedStreaming(10);

  otb::StandardWriterWatcher w4(writer, unmixer,"ISRAUnmixingImageFilter");

  writer->Update();

  return EXIT_SUCCESS;
}
